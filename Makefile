# Copyright (C) 2023-2024 Dmitry Ponomarev <ponomarevda96@gmail.com>
# Distributed under the terms of the GPL v3 license, available in the file LICENSE.

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD_DIR:=$(ROOT_DIR)/build
BUILD_OBJ_DIR:=$(BUILD_DIR)/obj
IS_SUBMODULE_INITIALIZED := $(shell find Libs/libparams -mindepth 1 | wc -l)

# Dronecan:
dronecan: check_submodules clean autogenerate_git_related_headers
	mkdir -p ${BUILD_OBJ_DIR}
	cd ${BUILD_OBJ_DIR} && cmake -DUSE_DRONECAN=1 ../.. && make
dronecan_debug: check_submodules clean autogenerate_git_related_headers
	mkdir -p ${BUILD_OBJ_DIR}
	cd ${BUILD_OBJ_DIR} && cmake -DUSE_DRONECAN=1 -DCMAKE_BUILD_TYPE=Debug ../.. && make
sitl_dronecan: check_submodules clean autogenerate_git_related_headers
	mkdir -p ${BUILD_OBJ_DIR}
	cd ${BUILD_OBJ_DIR} && cmake -DUSE_DRONECAN=1 -DUSE_PLATFORM_UBUNTU=1 ../.. && make

# Common:
check_submodules:
	@if [ "$(IS_SUBMODULE_INITIALIZED)" -eq 0 ]; then \
		echo "[Error]: submodules are empty. Please type: 'git submodule update --init --recursive'"; \
		exit 1; \
	else \
		echo "[INFO] Directory is fine"; \
	fi
upload:
	./scripts/tools/stm32/flash.sh ${BUILD_OBJ_DIR}/example.bin
run:
	./scripts/tools/can/vcan.sh slcan0
	./build/obj/example.out
autogenerate_git_related_headers:
	mkdir -p ${BUILD_DIR}/src
	./scripts/tools/stm32/generate_software_version.sh ${BUILD_DIR}/src
clean:
	-rm -fR ${BUILD_OBJ_DIR}/
distclean:
	-rm -fR ${BUILD_DIR}/
