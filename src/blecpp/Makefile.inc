
VPATH += \
	${BLECPP_ROOT} \

# compile .gatt descriptions
%.h: %.gatt
	python ${BTSTACK_ROOT}/tool/compile_gatt.py $< $@

lib-ble.a: iotble_uuid.h ${BT_CORE_OBJ} ${PORT_CORE_OBJ}
	${AR} rcs $@ $(filter-out iotble_uuid.h,$^)
