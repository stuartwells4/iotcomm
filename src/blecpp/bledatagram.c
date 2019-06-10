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

#include <iostream>
#include "bledatagram.h"
#include "btstack.h"
#include "btstack_defines.h"
#include "ble/att_db.h"
#include "ble/att_server.h"
#include "btstack_util.h"

bledatagram* bledatagram::priv_instance= NULL;
std::once_flag bledatagram::priv_init_instance_flag;

const uint8_t bledatagram::priv_service_uuid[] = { 0x01, 0x01, 0x3c, 0xa2, 0x8e, 0x10, 0x49, 0xf0, 0xba, 0x0C, 0x3c, 0xa8, 0xf9, 0xa4, 0xfb, 0x95 };
const uint8_t bledatagram::priv_char_status_uuid[] = { 0x11, 0x01, 0x3c, 0xa2, 0x8e, 0x10, 0x49, 0xf0, 0xba, 0x0C, 0x3c, 0xa8, 0xf9, 0xa4, 0xfb, 0x95 };
const uint8_t bledatagram::priv_char_command_uuid[] = { 0x11, 0x02, 0x3c, 0xa2, 0x8e, 0x10, 0x49, 0xf0, 0xba, 0x0C, 0x3c, 0xa8, 0xf9, 0xa4, 0xfb, 0x95 };

bledatagram::bledatagram()
{
  uint16_t sstart;
  uint16_t send;
  int found = gatt_server_get_get_handle_range_for_service_with_uuid128(priv_service_uuid,
                                                                        &sstart,
                                                                        &send);
  
  if (found) {
    priv_status = gatt_server_get_value_handle_for_characteristic_with_uuid128(sstart,
                                                                                      send,
                                                                                      priv_char_status_uuid);
    priv_status_conf = gatt_server_get_client_configuration_handle_for_characteristic_with_uuid128(sstart,
												   send,
												   priv_char_status_uuid);
    priv_command = gatt_server_get_value_handle_for_characteristic_with_uuid128(sstart,
										send,
										priv_char_command_uuid);
    priv_command_conf = gatt_server_get_client_configuration_handle_for_characteristic_with_uuid128(sstart,
												   send,
												   priv_char_command_uuid);
    priv_found = true;
  } else {
    priv_found = false;
  }
}

bledatagram::~bledatagram()
{
  
}
#if 0
void bledatagram::packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  if (packet_type == HCI_EVENT_PACKET) {
    switch (hci_event_packet_get_type(packet))
      {
      case BTSTACK_EVENT_STATE:
	if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
	  gap_local_bd_addr(bledatagram::priv_instance->priv_local_addr);
	  bledatagram::priv_instance->priv_db_path = "/tmp/bt_blecpp.tlv";
	  priv_tlv_impl = 
	    btstack_tlv_posix_init_instance(&priv_tlv_context,
					    bledatagram::priv_instance->priv_db_path.c_str());
	  btstack_tlv_set_instance(priv_tlv_impl, &priv_tlv_context);
	}
	break;
      default:
	break;
      }
  }
}
#endif

bledatagram& bledatagram::get_instance()
{
  // for a multithread system, assure init_singleton is called only once
  std::call_once(priv_init_instance_flag, &bledatagram::init_singleton);

  // return the instance
  return *priv_instance;
}

void bledatagram::init_singleton()
{
  priv_instance = new bledatagram;
}
