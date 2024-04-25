/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 
#ifndef SRC_PERIPHERY_PRESSURE_SENSOR_PRESSURE_SENSOR_HPP_
#define SRC_PERIPHERY_PRESSURE_SENSOR_PRESSURE_SENSOR_HPP_
#include "ms4525do.h"

struct PressureData {
    float temperature;
    float pressure;
};

class PressureDriverI2C {
public:
    PressureData data;

    int8_t init();
    int8_t get_data();
private:
    static bool inited;
};

#endif  // SRC_PERIPHERY_PRESSURE_SENSOR_PRESSURE_SENSOR_HPP_
