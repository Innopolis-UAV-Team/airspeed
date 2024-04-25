/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "pressure_sensor.hpp"

bool PressureDriverI2C::inited = false;

int8_t PressureDriverI2C::init() {
    if (inited) {
        return 0;
    }
    return ms4525doInit();
}

int8_t PressureDriverI2C::get_data() {
    if (ms4525CollectData() < 0) {
        return -1;
    }
    DifferentialPressureData raw_data = ms4525ParseCollectedData();
    data.pressure = raw_data.diff_pressure;
    data.temperature = raw_data.temperature;
    return 0;
}
