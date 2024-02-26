/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#ifndef SRC_MODULE_LIGHTS_HPP_
#define SRC_MODULE_LIGHTS_HPP_

#include "params.hpp"
#include "main.h"
#include "dronecan.h"
#include "../logger.hpp"
#include "../periphery/led/led.hpp"
#include "uavcan/equipment/indication/LightsCommand.h"

class LightsModule{
    // It is Singleton    
    static bool instance_initialized;

    uint8_t duty_cycle_ptc = 100;
    uint16_t blink_period = 0;
    uint16_t duty_cycle = 0;
    uint8_t max_intensity = 0;
    RgbSimpleColor default_color;
    static bool publish_error;
    static Logger logger;
    
    static void callback(CanardRxTransfer* transfer);
    void init();

protected:
    LightsModule(){};
    LightsModule(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity, RgbSimpleColor default_color);
public:
    static LightsModule instance;

    static LightsCommand_t command;
    static LightsCommand_t* command_ptr;
    GPIORgbLedDriver int_led_driver;
    PwmRgbLedDriver ext_led_driver;

    LightsModule(LightsModule &other) = delete;
    LightsModule& operator = (const LightsModule&) = delete;
    static LightsModule &get_instance(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity, RgbSimpleColor default_color);
    static LightsModule &get_instance();
    void spin_once();
    void reset_command();
    RgbSimpleColor change_color(RgbSimpleColor color);
};
#endif //SRC_MODULE_LIGHTS_HPP_