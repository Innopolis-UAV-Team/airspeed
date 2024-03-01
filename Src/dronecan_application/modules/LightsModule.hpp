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
    static uint8_t light_id;
    static uint8_t blink_type;
    uint8_t duty_cycle_ptc = 100;
<<<<<<< HEAD
    uint16_t toggle_period = 0;
    uint16_t duty_cycle = 0;
    uint8_t max_intensity = 0;
    RgbSimpleColor default_color;
    static bool publish_error;
=======
    uint16_t blink_period_ms = 0;
    uint16_t duty_cycle_ms = 0;
    uint8_t max_intensity = 0;
    RgbSimpleColor _current_color;
    static bool parsing_error;
>>>>>>> 37a65a2 (apply suggestions)
    static Logger logger;
    
    static void callback(CanardRxTransfer* transfer);
    void init();
    void update_params();

protected:
    LightsModule();
<<<<<<< HEAD
    // LightsModule(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity, RgbSimpleColor default_color);
public:
    // static LightsModule instance;
=======
    
public:
    static bool instance_initialized;

    static LightsModule instance;
>>>>>>> 37a65a2 (apply suggestions)

    static SingleLightCommand_t command;
    static bool is_cmd_received;
    GPIORgbLedDriver int_led_driver;
    PwmRgbLedDriver ext_led_driver;

    LightsModule(LightsModule &other) = delete;
    LightsModule& operator = (const LightsModule&) = delete;
<<<<<<< HEAD
    // static LightsModule &get_instance(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity, RgbSimpleColor default_color, uint8_t light_id = 0, uint8_t blink_type = 0);
    static LightsModule &get_instance(){
        static LightsModule instance;
        return instance;
    };
    void update_params();
=======
    static LightsModule &get_instance();
>>>>>>> 37a65a2 (apply suggestions)
    void spin_once();
    void reset_command();
    RgbSimpleColor change_color(RgbSimpleColor color);
};
#endif //SRC_MODULE_LIGHTS_HPP_