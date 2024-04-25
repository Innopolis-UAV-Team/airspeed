/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/

#ifndef SRC_MODULE_PRESSURE_MODULE_HPP_
#define SRC_MODULE_PRESSURE_MODULE_HPP_

#include "periphery/adc/adc.hpp"
#include "periphery/pressure_sensor/pressure_sensor.hpp"
#include "uavcan/equipment/air_data/RawAirData.h"
#include "module_status.hpp"
#include "params.hpp"
#include "logger.hpp"

enum class HeaterSateFlag {
    FLAG_HEATER_AVAILABLE      = 1,
    FLAG_HEATER_WORKING        = 2,
    FLAG_HEATER_OVERCURRENT    = 4,
    FLAG_HEATER_OPENCIRCUIT    = 8,
};

class PressureModule {
public:
    static PressureModule &get_instance();
    void spin_once();
    ModuleStatus status;

private:
    RawAirData_t air_data;
    int16_t offset;
    PressureDriverI2C pressure_driver;

    void init();
    void update_params();
    void publish_data();
    static Logger logger;

    uint8_t transfer_id;
    int8_t publish_error = 0;

    static PressureModule instance;
    static bool instance_initialized;
    PressureModule() {}

    PressureModule(const PressureModule &other) = delete;
    void operator=(const PressureModule &) = delete;
};

#endif  // SRC_MODULE_PRESSURE_MODULE_HPP_


