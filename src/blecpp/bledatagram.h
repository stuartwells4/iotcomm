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
#include <stdint.h>
#include <stdbool.h>
#include "bledatagram.h"
#include "btstack.h"
#include "btstack_defines.h"
#include "ble/att_db.h"
#include "ble/att_server.h"
#include "btstack_util.h"

class bledatagram
{
 public:
  bledatagram();
  ~bledatagram();

 private:
  static void priv_packet_handler(uint8_t packet_type, 
				  uint16_t channel, 
				  uint8_t *packet,
				  uint16_t size);
  static uint16_t priv_read_callback(hci_con_handle_t con_handle,
				     uint16_t handle,
				     uint16_t offset,
				     uint8_t * buffer,
				     uint16_t buffer_size);
  static int priv_write_callback(hci_con_handle_t con_handle,
				 uint16_t gandle,
				 uint16_t mode,
				 uint16_t offset,
				 uint8_t * buffer,
				 uint16_t buffer_size);
  static const uint8_t priv_service_uuid[];
  static const uint8_t priv_char_status_uuid[];
  static const uint8_t priv_char_command_uuid[];
  uint16_t priv_status;
  uint16_t priv_status_conf;
  uint16_t priv_command;
  uint16_t priv_command_conf;
  bool priv_found;
  bool priv_status_notify;
  bool priv_command_notify;
  btstack_packet_callback_registration_t priv_packet_service;
  att_service_handler_t priv_att_service;

  // Singleton instance.
 public:
  static bledatagram& get_instance();

 private:
  static bledatagram* priv_instance;
  static std::once_flag priv_init_instance_flag;
  static void init_singleton();
};
