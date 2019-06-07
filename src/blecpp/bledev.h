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

#include <iostream>
#include <mutex>
#include <stdbool.h>
#include "blecpp_uuid.h"
#include "btstack.h"
#include "bluetooth.h"

#define BLE_ADVERT_LENGTH 31
#define BLE_RESPONSE_LENGTH 32

class bledev
{
 public:
  bledev();
  ~bledev();

  // Start the BLE Device
  void start(void);
  void submit_battery(uint8_t level);
  void submit_manufacture(const std::string& str);
  void submit_model_number(const std::string& str);
  void submit_firmware(const std::string& str);
  void submit_software(const std::string& str);
  void submit_serial(const std::string& str);
  void submit_advert_name(const std::string& name);

 private:
  static void packet_handler(uint8_t packet_type,
			     uint16_t channel,
			     uint8_t *packet,
			     uint16_t size);

  static uint16_t read_callback(hci_con_handle_t con_handle, 
				uint16_t att_handle, 
				uint16_t offset, 
				uint8_t * buffer, 
				uint16_t buffer_size);
  
  static int write_callback(hci_con_handle_t con_handle,
			    uint16_t att_handle, 
			    uint16_t mode, 
			    uint16_t offset, 
			    uint8_t *buffer, 
			    uint16_t buffer_size);

 private:
  bool priv_notification_enabled;
  uint8_t priv_battery_level;
  std::string priv_str_manufacture;
  std::string priv_str_model;
  std::string priv_str_firmware;
  std::string priv_str_software;
  std::string priv_str_serial;
  std::string priv_str_devicename;
  uint8_t priv_advert_data [BLE_ADVERT_LENGTH];
  uint8_t priv_reponse_data [BLE_RESPONSE_LENGTH];
  uint8_t priv_advert_length;
  uint8_t priv_response_length;
  btstack_packet_callback_registration_t hci_event_callback_registration;

  enum {
    DATA_TYPE=0x01,
    SCAN_INCOMPLETE_UUID16_LIST=0x02,
    SCAN_COMPLETE_UUID16_LIST=0x03,
    DISCO_NO_BR_EDR=0x06,
    ADVERT_SHORT_NAME=0x08,
    ADVERT_COMPLETE_NAME=0x09,
    SCAN_MANUFACTURER_DATA=0xFF
  } BLE_ADVERT_INFO;
  // Singleton instance.
 public:
  static bledev& get_instance();

 private:
  static bledev* priv_instance;
  static std::once_flag priv_init_instance_flag;

  static void init_singleton();
};
