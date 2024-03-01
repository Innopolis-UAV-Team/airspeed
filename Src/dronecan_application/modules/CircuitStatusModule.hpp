/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#ifndef SRC_MODULE_CIRCUIT_STATUS_HPP_
#define SRC_MODULE_CIRCUIT_STATUS_HPP_

#include "periphery/led/led.hpp"
#include "uavcan/equipment/power/CircuitStatus.h"
#include "uavcan/equipment/temperature/Temperature.h"
#include "periphery/adc/adc.hpp"
#include "LightsModule.hpp"
#include "logger.hpp"


class CircuitStatusModule {
    static CircuitStatusModule instance;
    static bool instance_initialized;
    static Logger logger;
    CircuitStatus_t circuit_status = {};
    Temperature_t temperature_status = {};

private:
    uint8_t circuit_status_transfer_id = 0;
    uint8_t temperature_transfer_id = 0;

    uint16_t vol_raw = 0;
    uint16_t temp_raw = 0;
    uint16_t cur_raw = 0;

    uint16_t v5 = 0;
    int8_t publish_error = 0;
    AdcPeriphery adc;
    // CircuitStatusModule(LightsModule* light_module);
    CircuitStatusModule(){};
public:
    float temp = 0;
    float v5_f = 0.0;
    float vol = 0;
    float curr = 0;
    CircuitStatusModule(CircuitStatusModule &other) = delete;
    void operator=(const CircuitStatusModule &) = delete;

    static CircuitStatusModule &get_instance();
    void init();
    void spin_once();
};

#endif //SRC_MODULE_CIRCUIT_STATUS_HPP_