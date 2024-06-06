# Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
# Distributed under the terms of the GPL v3 license, available in the file LICENSE.

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH SRC_DIR)
cmake_path(GET SRC_DIR PARENT_PATH ROOT_DIR)

add_definitions(-DAPP_VERSION_MAJOR=0)
add_definitions(-DAPP_VERSION_MINOR=2)
add_definitions(-DHW_VERSION_MAJOR=2)
add_definitions(-DHW_VERSION_MINOR=1)
add_definitions(-DFLASH_DRIVER_STORAGE_FILE="${CMAKE_CURRENT_LIST_DIR}/default_params.yml")
add_definitions(-DFLASH_DRIVER_SIM_STORAGE_FILE="${BUILD_SRC_DIR}/sim_params.yml")

include(${ROOT_DIR}/Libs/Dronecan/CMakeLists.txt)
include(${ROOT_DIR}/Src/dronecan_application/modules/PWM/CMakeLists.txt)
include(${ROOT_DIR}/Src/dronecan_application/modules/RangeFinder/CMakeLists.txt)
include(${ROOT_DIR}/Src/dronecan_application/modules/Lights/CMakeLists.txt)
include(${ROOT_DIR}/Src/dronecan_application/modules/DifferentialPressure/CMakeLists.txt)
include(${ROOT_DIR}/Src/dronecan_application/modules/CircuitStatus/CMakeLists.txt)

set(applicationSourceCode
    ${libparamsSrc}
    ${DRONECAN_SOURCES}
    ${RANGEFINDER_SOURCES}
    ${DIFFPRESSURE_SOURCES}
    ${PWM_SOURCES}
    ${LIGHTS_SOURCES}
    ${CIRCUITSTATUS_SOURCES}
    ${ROOT_DIR}/Src/dronecan_application/logger.cpp
    ${ROOT_DIR}/Src/dronecan_application/application.cpp
)
set(applicationHeaders
    ${libparamsHeaders}
    ${DRONECAN_HEADERS}
    ${RANGEFINDER_HEADERS}
    ${DIFFPRESSURE_HEADERS}
    ${PWM_HEADERS}
    ${LIGHTS_HEADERS}
    ${CIRCUITSTATUS_HEADERS}
    ${ROOT_DIR}/Src
    ${ROOT_DIR}/Src/dronecan_application
)

set(LIBPARAMS_PARAMS
    ${CMAKE_CURRENT_LIST_DIR}/params.yaml
    ${RANGEFINDER_PARAMS}
)
