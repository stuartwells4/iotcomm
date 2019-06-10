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
#include "blesetup.h"

blesetup* blesetup::priv_instance= NULL;
std::once_flag blesetup::priv_init_instance_flag;

blesetup::blesetup()
{
}

blesetup::~blesetup()
{
  
}

void blesetup::start(void)
{
  btstack_memory_init();
  btstack_run_loop_init(btstack_run_loop_posix_get_instance());

  hci_dump_open("/tmp/hci_dump.txt", HCI_DUMP_PACKETLOGGER);
  hci_init(hci_transport_usb_instance(), NULL);

  hci_event_callback_registration.callback = &packet_handler;
  hci_add_event_handler(&hci_event_callback_registration);

  signal(SIGINT, sigint_handler);
}

void blesetup::power_on(void)
{
  hci_power_control(HCI_POWER_ON);
}

void blesetup::process(void)
{
  printf("process start\n");
  btstack_run_loop_execute();
  printf("I am still here\n");
}

static btstack_tlv_posix_t priv_tlv_context;
static const btstack_tlv_t *priv_tlv_impl;

void blesetup::packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
  if (packet_type == HCI_EVENT_PACKET) {
    switch (hci_event_packet_get_type(packet))
      {
      case BTSTACK_EVENT_STATE:
	if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
	  gap_local_bd_addr(blesetup::priv_instance->priv_local_addr);
	  blesetup::priv_instance->priv_db_path = "/tmp/bt_blecpp.tlv";
	  priv_tlv_impl = 
	    btstack_tlv_posix_init_instance(&priv_tlv_context,
					    blesetup::priv_instance->priv_db_path.c_str());
	  btstack_tlv_set_instance(priv_tlv_impl, &priv_tlv_context);
	}
	break;
      default:
	break;
      }
  }
}

void blesetup::sigint_handler(int param)
{
  UNUSED(param);

  std::cout << "SIGINT received, shutting down\n";
  log_info("sigint_handler: shutting down");

  hci_power_control(HCI_POWER_OFF);
  hci_close();
  log_info("Good bye, see you.\n");
  exit(0);
}

blesetup& blesetup::get_instance()
{
  // for a multithread system, assure init_singleton is called only once
  std::call_once(priv_init_instance_flag, &blesetup::init_singleton);

  // return the instance
  return *priv_instance;
}

void blesetup::init_singleton()
{
  priv_instance = new blesetup;
}
