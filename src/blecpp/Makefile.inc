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

VPATH += \
	${BLECPP_ROOT}

CFLAGS += \
	-I${BLECPP_ROOT}

PROJECT_BTE_SERVICE += \
		    battery_service_server.o \
		    device_information_service_server.o

# compile .gatt descriptions
%.h: %.gatt
	python ${BTSTACK_ROOT}/tool/compile_gatt.py $< $@

#blecpp.o: blecpp.c
#	${CXX} $(filter-out iotble_uuid.h,$^) ${CFLAGS} ${LDFLAGS} -o $@

blecpp: blecpp_uuid.h bledev.c main.c libiotble.a
	${CXX} -c $(filter-out blecpp_uuid.h,$^) ${CFLAGS}
	${CXX} bledev.o main.o ${LDFLAGS} libiotble.a -o $@

libiotble.a: blecpp_uuid.h ${BT_CORE_OBJ} ${PORT_CORE_OBJ} ${PROJECT_BTE_SERVICE}
	${AR} rcs $@ $(filter-out blecpp_uuid.h,$^)

