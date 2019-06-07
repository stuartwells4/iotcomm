#
# IOTCOMM
#
# Copyright (C) 2019 Stuart Wells <swells@stuartwells.net>
# The MIT License (MIT)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

IOT_ROOT = ../../
BTSTACK_ROOT = ${IOT_ROOT}/thirdparty/btstack
BLECPP_ROOT = ${IOT_ROOT}/src/blecpp

# Code paths for btstack
VPATH += \
	${BTSTACK_ROOT}/platform/embedded \
	${BTSTACK_ROOT}/platform/libusb \
	${BTSTACK_ROOT}/platform/posix \
	${BTSTACK_ROOT}/platform/libusb \
	${BTSTACK_ROOT}/chipset/csr \
	${BTSTACK_ROOT}/src \
	${BTSTACK_ROOT}/src/ble \
	${BTSTACK_ROOT}/src/ble/gatt-service \
	${BTSTACK_ROOT}/src/classic \
	${BTSTACK_ROOT}/3rd-party/md5 \
	${BTSTACK_ROOT}/3rd-party/micro-ecc \
	${BTSTACK_ROOT}/3rd-party/bluedroid/decoder/srce \
	${BTSTACK_ROOT}/3rd-party/bluedroid/encoder//srce \
	${BTSTACK_ROOT}/3rd-party/yxml \
	${BTSTACK_ROOT}/3rd-party/hxcmod-player

# Include Paths for btstack
CFLAGS += \
	-I. \
	-I${BTSTACK_ROOT}/3rd-party/tinydir \
	-I${BTSTACK_ROOT}/platform/posix \
	-I${BTSTACK_ROOT}/platform/embedded \
	-I${BTSTACK_ROOT}/src/ble \
	-I${BTSTACK_ROOT}/src/classic \
	-I${BTSTACK_ROOT}/src \
	-I${BTSTACK_ROOT}/3rd-party/hxcmod-player \
	-I${BTSTACK_ROOT}/3rd-party/md5 \
	-I${BTSTACK_ROOT}/3rd-party/micro-ecc \
	-I${BTSTACK_ROOT}/3rd-party/bluedroid/decoder/include \
	-I${BTSTACK_ROOT}/3rd-party/bluedroid/encoder/include \
	-I${BTSTACK_ROOT}/3rd-party/yxml

CFLAGS  += $(shell pkg-config libusb-1.0 --cflags)

# Linker flags
LDFLAGS += $(shell pkg-config libusb-1.0 --libs)

# Combined CORE, COMMON, CLASSIC, ATT, SERVER, SM files into one.
BT_CORE += \
	btstack_memory.c \
	btstack_linked_list.c \
	btstack_memory_pool.c \
	btstack_run_loop.c \
	btstack_util.c \
	att_db.c \
	att_server.c \
	gatt_client.c \
	ad_parser.c \
	hci.c \
	hci_cmd.c \
	hci_dump.c \
	l2cap.c \
	l2cap_signaling.c \
	btstack_tlv.c \
	btstack_crypto.c \
	uECC.c \
	rfcomm.c \
	sm.c \
	att_dispatch.c

PORT_CORE += \
	hci_transport_h2_libusb.c \
	btstack_run_loop_posix.c \
	le_device_db_fs.c \
	btstack_link_key_db_fs.c

# o name for C
BT_CORE_OBJ = $(BT_CORE:.c=.o)
PORT_CORE_OBJ = $(PORT_CORE:.c=.o)

include ${BLECPP_ROOT}/Makefile.inc

clean:
	rm -f  *.o *.out *.hex *.exe *.wav *.sbc *.a
	rm -f  blecpp_uuid.h*
	rm -rf *.dSYM
	rm -f blecpp
	rm -rf ${BTSTACK_ROOT}/src/classic/*.o
	rm -rf ${BTSTACK_ROOT}/platform/embedded/*.o
	rm -rf ${BTSTACK_ROOT}/src/ble/gatt-service/*.o
	rm -rf ${BTSTACK_ROOT}/example/*.o



