/*
 IOTCOMM

 Copyright (C) 2019 Stuart Wells <swells@stuartwells.net>
 The MIT License (MIT)

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#include "iotble.h"

iotble* iotble::priv_instance= NULL;;
std::once_flag iotble::priv_init_instance_flag;

iotble::iotble()
{
  printf("The initial function is called\n");
}

iotble::~iotble()
{
  printf("The deinit function is called\n");
}

iotble& iotble::get_instance()
{
  // for a multithread system, assure init_singleton is called only once
  std::call_once(priv_init_instance_flag, &iotble::init_singleton);

  // return the instance
  return *priv_instance;
}

void iotble::init_singleton()
{
  priv_instance = new iotble;
}

int main(int argc, char *argv[])
{
  iotble ble = iotble::get_instance(); 

  //  printf("Do we have an instance %p\n", ble);

  return 0;
}


