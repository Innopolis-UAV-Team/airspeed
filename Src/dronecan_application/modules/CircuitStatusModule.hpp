
#ifndef SRC_MODULE_CIRCUIT_STATUS_HPP_
#define SRC_MODULE_CIRCUIT_STATUS_HPP_

#include "periphery/led/led.hpp"
#include "uavcan/equipment/power/CircuitStatus.h"
#include "uavcan/equipment/temperature/Temperature.h"
#include "periphery/adc/adc.hpp"
#include "LightsModule.hpp"
#include "logger.hpp"


class CircuitStatusModule{
    static CircuitStatusModule* entity;
    static Logger logger;
    CircuitStatus_t circuit_status = {};
    Temperature_t temperature_status = {};

    // CircuitStatusModule* circuit_status_module;
private:
    uint8_t circuit_status_pub_id = 0;
    uint8_t temperature_pub_id = 0;

    uint16_t vol_raw = 0;
    uint16_t temp_raw = 0;
    uint16_t cur_raw = 0;

    uint16_t v5 = 0;
    int8_t publish_error = 0;
    AdcPeriphery adc;
    CircuitStatusModule(LightsModule* light_module_ptr);

public:
    LightsModule* light_module;

    float temp = 0;
    float v5_f = 0.0;
    float vol = 0;
    float curr = 0;
    RgbSimpleColor color;
    CircuitStatusModule(CircuitStatusModule &other) = delete;
    void operator=(const CircuitStatusModule &) = delete;

    static CircuitStatusModule *GetInstance(LightsModule* light_module);
    void init();
    void spin_once();
};
#endif //SRC_MODULE_CIRCUIT_STATUS_HPP_