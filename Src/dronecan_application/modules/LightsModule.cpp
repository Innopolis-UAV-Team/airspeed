/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "LightsModule.hpp"

// Initialize the static member variables
char buffer[90];
uint8_t LightsModule::light_id = 0;
uint8_t LightsModule::toggle_type = 0;
LightsModule LightsModule::instance = LightsModule();
bool LightsModule::instance_initialized = false;

bool LightsModule::is_cmd_received = false;
SingleLightCommand_t LightsModule::command = {};
bool LightsModule::parsing_error = 0;
Logger LightsModule::logger = Logger("LightsModule");


LightsModule::LightsModule(): int_led_driver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE),  ext_led_driver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6){
    update_params();
    instance.init();
    instance_initialized = true;
};


LightsModule &LightsModule::get_instance() {
    if (!instance_initialized) {
        if (HAL_GetTick()%1000 == 0){
            logger.log_debug("instance not initialized!");
        }
    }
    return instance;
}


void LightsModule::update_params(){
    char buffer[90];

    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);

    toggle_type = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_TYPE);
    max_intensity = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    light_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHT_ID);
    toggle_period_ms = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);
    
    if (toggle_type == 1) {
        duty_cycle_ptc = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
        float frac = (duty_cycle_ptc / 100.0);
        duty_cycle_ms = frac * toggle_period_ms;
    } else {
        duty_cycle_ms = toggle_period_ms;
    }

    _current_color = RgbSimpleColor(default_color);
}


void LightsModule::init() {

    int_led_driver = GPIORgbLedDriver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE);

    int_led_driver.toggle_period_ms = 1000;
    int_led_driver.duty_cycle_ms = 500;

    ext_led_driver = PwmRgbLedDriver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6);
    ext_led_driver.toggle_period_ms = toggle_period_ms;
    ext_led_driver.duty_cycle_ms = duty_cycle_ms;

    ext_led_driver.set_intensity(max_intensity);
    
    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    if (sub_id < 0) {
        logger.log_debug("uavcanSubscribe failed");
    }

    RgbSimpleColor color_int = RgbSimpleColor::BLUE_COLOR;
    int_led_driver.set(color_int);
    ext_led_driver.set(_current_color);
}

void LightsModule::spin_once() {
    if (is_cmd_received) {
        Rgb565Color color={};
        color = {
            command.color.red, 
            command.color.green, 
            command.color.blue
            };
        ext_led_driver.set(color);
    }
    
    if (toggle_type == 2){
        uint8_t intensity = max_intensity * ((HAL_GetTick() % toggle_period_ms) / float (toggle_period_ms));
        ext_led_driver.set_intensity(intensity);
    }

    if (HAL_GetTick() % 5000 == 0) {
        update_params();
        init();
    }
    if (HAL_GetTick() % 1000 == 0) {
        sprintf(buffer, "%d %d", ext_led_driver.duty_cycle_ms, ext_led_driver.toggle_period_ms);
        logger.log_debug(buffer);
    }
    int_led_driver.spin();
    ext_led_driver.spin();
}

void LightsModule::callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        for (uint8_t i = 0; i < raw_command.number_of_commands; i++) {
            if (raw_command.commands[i].light_id == light_id){
                command = raw_command.commands[i];
                is_cmd_received = true;
                break;
            }
        }
    }
}

RgbSimpleColor LightsModule::change_color(RgbSimpleColor color) {
    switch (color)
    {
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
}