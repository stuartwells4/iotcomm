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

    priv_att_service.start_handle = sstart;
    priv_att_service.end_handle = send;
    priv_att_service.read_callback = &bledatagram::priv_read_callback;
    priv_att_service.write_callback = &bledatagram::priv_write_callback;
    att_server_register_service_handler(&priv_att_service);

    priv_packet_service.callback = &priv_packet_handler;
    hci_add_event_handler(&priv_packet_service);

    priv_found = true;
  } else {
    priv_found = false;
  }
  priv_status_notify = false;
  priv_command_notify = false;

}

bledatagram::~bledatagram()
{
  
}

void bledatagram::priv_packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  if (packet_type == HCI_EVENT_PACKET) {
    if (HCI_EVENT_PACKET == packet_type)
      {
	switch (hci_event_packet_get_type(packet))
	  {
	  case HCI_EVENT_DISCONNECTION_COMPLETE:
	    bledatagram::priv_instance->priv_status_notify = false;
	    bledatagram::priv_instance->priv_command_notify = false;
	    break;
	  default:
	    break;
	  }
      }
  }
}

uint16_t bledatagram::priv_read_callback(hci_con_handle_t con_handle, 
					 uint16_t att_handle, 
					 uint16_t offset, 
					 uint8_t *buffer, 
					 uint16_t buffer_size)
{
  return 0;
}

int bledatagram::priv_write_callback(hci_con_handle_t con_handle,
				     uint16_t att_handle, 
				     uint16_t mode, 
				     uint16_t offset, 
				     uint8_t *buffer, 
				     uint16_t buffer_size)
{
  return 0;
}


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
