/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file
 *LICENSE.
 ***/
#ifndef SRC_MODULE_LIGHTS_HPP_
#define SRC_MODULE_LIGHTS_HPP_

#include "../../logger.hpp"
#include "drivers/led.hpp"
#include "dronecan.h"
#include "main.h"
#include "params.hpp"
#include "uavcan/equipment/indication/LightsCommand.h"

class LightsModule {
   public:
    static SingleLightCommand_t command;
    static bool is_cmd_received;

    GPIORgbLedDriver int_led_driver;
    PwmRgbLedDriver ext_led_driver;

    static uint32_t ttl_current_cmd_ms;
    static uint16_t ttl_cmd;

    void spin_once();
    void reset_command();
    void update_params();
    void publish_command();
    static LightsModule& get_instance();
    RgbSimpleColor change_color(RgbSimpleColor color);

   protected:
    LightsModule();

   private:
    static bool instance_initialized;
    static LightsModule instance;

    static uint8_t light_id;
    static uint8_t toggle_type;

    uint8_t duty_cycle_ptc = 100;
    uint16_t toggle_period_ms = 0;
    uint16_t duty_cycle_ms = 0;
    uint8_t max_intensity = 0;

    bool verbose;
    Rgb565Color _default_color;
    static Logger logger;

    void apply_params();
    static void callback(CanardRxTransfer* transfer);
    LightsModule& operator=(const LightsModule&) = delete;
    LightsModule(const LightsModule& other) = delete;
};

#endif  // SRC_MODULE_LIGHTS_HPP_
