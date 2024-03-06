/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "LightsModule.hpp"

bool LightsModule::instance_initialized = false;
LightsModule LightsModule::instance = LightsModule();

uint8_t LightsModule::light_id = 0;
uint8_t LightsModule::toggle_type = 0;

SingleLightCommand_t LightsModule::command = {};
uint32_t LightsModule::ttl_current_cmd_ms = 0;
uint16_t LightsModule::ttl_cmd = 0;

Logger LightsModule::logger = Logger("LightsModule");

LightsModule::LightsModule(): int_led_driver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE),  ext_led_driver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6) {
    update_params();
    instance_initialized = true;
};


LightsModule &LightsModule::get_instance() {
    instance.update_params();
    return instance;
}


void LightsModule::reset_command() {
    command = {};
}


void LightsModule::spin_once() {
    Rgb565Color color = _default_color;
    if (HAL_GetTick() < ttl_current_cmd_ms) {
        color = Rgb565Color{
            command.color.red, 
            command.color.green, 
            command.color.blue
            };
        ext_led_driver.set(color);
    } else {
        if (toggle_type == 2) {
            uint8_t intensity = max_intensity * ((HAL_GetTick() % toggle_period_ms) / float (toggle_period_ms));
            ext_led_driver.set_intensity(intensity);
        }
        ext_led_driver.spin(color);
    }

    static uint32_t next_update_ms = 10;
    if (HAL_GetTick() > next_update_ms) {
        next_update_ms += 100;
        update_params();
    }

    int_led_driver.spin(Rgb565Color{0, 0, 1});

    if (verbose) {  
        publish_command();
    }
}

void LightsModule::publish_command() {
    static uint32_t next_pub_ms = 100;
    if (next_pub_ms < HAL_GetTick()) {
    next_pub_ms += 10;

    uint16_t red = PwmPeriphery::get_duration(PwmPin::PWM_4) * 0.031f;
    uint16_t green = PwmPeriphery::get_duration(PwmPin::PWM_3) * 0.063f;
    uint16_t blue = PwmPeriphery::get_duration(PwmPin::PWM_6) * 0.031f;

    static uint8_t transfer_id = 0;
    LightsCommand_t cmd{};
    cmd.number_of_commands = 1;
    cmd.commands[0].light_id = 1;
    cmd.commands[0].color.red = 31 - red;
    cmd.commands[0].color.green = 63 - green;
    cmd.commands[0].color.blue = 31 - blue;
    dronecan_equipment_indication_lights_command_publish(&cmd, &transfer_id);
    transfer_id++;
    }

}

void LightsModule::callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        for (uint8_t i = 0; i < raw_command.number_of_commands; i++) {
            if (raw_command.commands[i].light_id == light_id) {
                command = raw_command.commands[i];
                ttl_current_cmd_ms = HAL_GetTick() + ttl_cmd;
                break;
            }
        }
    }
}


void LightsModule::update_params() {
    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);

    toggle_type = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_TYPE);
    max_intensity = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    light_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_ID);
    toggle_period_ms = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_PERIOD_MS);
    
    if (toggle_type == 1) {
        duty_cycle_ptc = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
        float frac = (duty_cycle_ptc / 100.0);
        duty_cycle_ms = frac * toggle_period_ms;
    } else {
        duty_cycle_ms = toggle_period_ms;
    }
    _default_color = Rgb565Color::from_rgb_simple_color(RgbSimpleColor(default_color));
    verbose = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_VERBOSE);
    instance.apply_params();
    ttl_cmd = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_TTL);
}

void LightsModule::apply_params() {
    int_led_driver = GPIORgbLedDriver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE);

    int_led_driver.toggle_period_ms = 1000;
    int_led_driver.duty_cycle_ms = 500;

    ext_led_driver = PwmRgbLedDriver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6);
    ext_led_driver.toggle_period_ms = toggle_period_ms;
    ext_led_driver.duty_cycle_ms = duty_cycle_ms;

    ext_led_driver.set_intensity(max_intensity);
    
    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND_SIGNATURE, UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND_ID, callback);
    if (sub_id < 0) {
    }

    RgbSimpleColor color_int = RgbSimpleColor::BLUE_COLOR;
    int_led_driver.set(color_int);
}


RgbSimpleColor LightsModule::change_color(RgbSimpleColor color) {

    switch (color) {
    case RgbSimpleColor::RED_COLOR:
        color = RgbSimpleColor::BLUE_COLOR;
        break;
    
    case RgbSimpleColor::BLUE_COLOR:
        color = RgbSimpleColor::GREEN_COLOR;
        break;
    
    case RgbSimpleColor::GREEN_COLOR:
        color = RgbSimpleColor::MAGENTA_COLOR;
        break;
    
    case RgbSimpleColor::MAGENTA_COLOR:
        color = RgbSimpleColor::YELLOW_COLOR;
        break;
    
    case RgbSimpleColor::YELLOW_COLOR:
        color = RgbSimpleColor::CYAN_COLOR;
        break;
    
    case RgbSimpleColor::CYAN_COLOR:
        color = RgbSimpleColor::WHITE_COLOR;
        break;
    
    default:
        color = RgbSimpleColor::RED_COLOR;
        break;
    }
    return color;
}