#include "bledev.h"

int main(int argc, char *argv[])
{
  bledev ble = bledev::get_instance(); 

  //  printf("Do we have an instance %p\n", ble);

  return 0;
}
