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

#include <string.h>

#include "bledev.h"
#include "btstack_defines.h"
#include "ble/gatt-service/battery_service_server.h"
#include "ble/gatt-service/device_information_service_server.h"

#define RFCOMM_SERVER_CHANNEL 1

#define DEFAULT_COMPANY "Default Company, LLC"
#define DEFAULT_MODEL "Default Model"
#define DEFAULT_FIRMWARE "Default Firmware"
#define DEFAULT_SOFTWARE "Default Software"
#define DEFAULT_SERIAL "Default Serial"
#define DEFAULT_ADVERT_NAME "Default Advert Name"
#define DEFAULT_DEVICE_NAME "Default Device Name"

bledev* bledev::priv_instance= NULL;
std::once_flag bledev::priv_init_instance_flag;

bledev::bledev()
{
  priv_notification_enabled = false;
  priv_battery_level = 0;
  priv_str_manufacture = DEFAULT_COMPANY;
  priv_str_model = DEFAULT_MODEL;
  priv_str_firmware = DEFAULT_FIRMWARE;
  priv_str_software = DEFAULT_SOFTWARE;
  priv_str_serial = DEFAULT_SERIAL;
  priv_str_devicename = DEFAULT_DEVICE_NAME;

  priv_response_length = 0;
  priv_reponse_data [priv_response_length++] = 0x03;
  priv_reponse_data [priv_response_length++] = SCAN_INCOMPLETE_UUID16_LIST;
  priv_reponse_data [priv_response_length++] = 0x0a;
  priv_reponse_data [priv_response_length++] = 0x18;

  submit_advert_name(DEFAULT_ADVERT_NAME);
}

bledev::~bledev()
{
}

void bledev::start(void)
{
  l2cap_init();

  rfcomm_init();
  rfcomm_register_service(packet_handler, RFCOMM_SERVER_CHANNEL, 0xffff);
  le_device_db_init();
  sm_init();

  att_server_init(profile_data, read_callback, write_callback);

  battery_service_server_init(priv_battery_level);
  device_information_service_server_init();
  device_information_service_server_set_manufacturer_name(priv_str_manufacture.c_str());
  device_information_service_server_set_model_number(priv_str_model.c_str());
  device_information_service_server_set_firmware_revision(priv_str_firmware.c_str());
  device_information_service_server_set_software_revision(priv_str_software.c_str());
  device_information_service_server_set_serial_number(priv_str_serial.c_str());

  bd_addr_t null_addr;
  memset(null_addr, 0, 6);

  gap_set_local_name(priv_str_devicename.c_str());
  gap_advertisements_set_params(0x0030, 0x0030, 0, 0, null_addr, 0x07, 0x00);
  gap_advertisements_set_data(priv_advert_length, priv_advert_data);
  gap_scan_response_set_data(priv_response_length, priv_reponse_data);
  gap_advertisements_enable(1);

  hci_event_callback_registration.callback = &packet_handler;
  hci_add_event_handler(&hci_event_callback_registration);

  att_server_register_packet_handler(packet_handler);
}

void bledev::submit_battery(uint8_t level)
{
  priv_battery_level = level;
  battery_service_server_set_battery_value(priv_battery_level);
}

void bledev::submit_manufacture(const std::string& str)
{
  priv_str_manufacture = str;
  device_information_service_server_set_manufacturer_name(priv_str_manufacture.c_str());
}

void bledev::submit_model_number(const std::string& str)
{
  priv_str_model = str;
  device_information_service_server_set_model_number(priv_str_model.c_str());
}

void bledev::submit_firmware(const std::string& str)
{
  priv_str_firmware = str;
  device_information_service_server_set_firmware_revision(priv_str_firmware.c_str());
}

void bledev::submit_software(const std::string& str)
{
  priv_str_software = str;
  device_information_service_server_set_software_revision(priv_str_software.c_str());
}

void bledev::submit_serial(const std::string& str)
{
  priv_str_serial = str;
  device_information_service_server_set_serial_number(priv_str_serial.c_str());
}

void bledev::submit_advert_name(const std::string& name)
{
  int length;

  priv_advert_length = 0;
  priv_advert_data [priv_advert_length++] = 0x02;
  priv_advert_data [priv_advert_length++] = DATA_TYPE;
  priv_advert_data [priv_advert_length++] = DISCO_NO_BR_EDR;
  length = name.length();
  if (length > (BLE_ADVERT_LENGTH - priv_advert_length)) {
    length = BLE_ADVERT_LENGTH - 2;
    priv_advert_data [priv_advert_length++] = length;
    priv_advert_data [priv_advert_length++] = ADVERT_SHORT_NAME;
  } else {
    priv_advert_data [priv_advert_length++] = length;
    priv_advert_data [priv_advert_length++] = ADVERT_COMPLETE_NAME;
  }

  strncpy((char *)&priv_advert_data [priv_advert_length], name.c_str(), length);
  priv_advert_length += length;
}


void bledev::packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  UNUSED(channel);
  UNUSED(size);

  if (HCI_EVENT_PACKET == HCI_EVENT_PACKET) {
    switch (hci_event_packet_get_type(packet))
      {
      case HCI_EVENT_DISCONNECTION_COMPLETE:
	gap_discoverable_control(1);
	gap_connectable_control(1);
	gap_advertisements_enable(1);
	bledev::priv_instance->priv_notification_enabled = false;
	break;
      }
  }
}

uint16_t bledev::read_callback(hci_con_handle_t con_handle, 
			       uint16_t att_handle, 
			       uint16_t offset, 
			       uint8_t *buffer, 
			       uint16_t buffer_size)
{
  return 0;
}

int bledev::write_callback(hci_con_handle_t con_handle,
			  uint16_t att_handle, 
			  uint16_t mode, 
			  uint16_t offset, 
			  uint8_t *buffer, 
			  uint16_t buffer_size)
{
  return 0;
}

bledev& bledev::get_instance()
{
  // for a multithread system, assure init_singleton is called only once
  std::call_once(priv_init_instance_flag, &bledev::init_singleton);

  // return the instance
  return *priv_instance;
}

void bledev::init_singleton()
{
  priv_instance = new bledev;
}



