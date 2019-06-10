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

#pragma once
#include "btstack.h"
#include "bluetooth.h"
#include "btstack_link_key_db_fs.h"
#include "btstack_tlv_posix.h"
#include "btstack_run_loop.h"
#include "btstack_run_loop_posix.h"
#include "btstack_debug.h"
#include "btstack_event.h"
#include "btstack_memory.h"
#include "btstack_config.h"
#include "hal_led.h"
#include "hci.h"
#include "hci_dump.h"

class blesetup
{
 public:
  blesetup();
  ~blesetup();

  void start(void);
  void power_on(void);
  void process(void);

 private:
  static void sigint_handler(int param);
  static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

 private:
  std::string priv_db_path;
  bd_addr_t priv_local_addr;
  btstack_packet_callback_registration_t hci_event_callback_registration;

  // Singleton instance.
 public:
  static blesetup& get_instance();

 private:
  static blesetup* priv_instance;
  static std::once_flag priv_init_instance_flag;

  static void init_singleton();
};
