/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "LightsModule.hpp"

// Initialize the static member variables
char buffer[90];
uint8_t LightsModule::light_id = 0;
uint8_t LightsModule::blink_type = 0;
// LightsModule LightsModule::instance = LightsModule();
bool LightsModule::instance_initialized = false;

SingleLightCommand_t* LightsModule::command_ptr = nullptr;
SingleLightCommand_t LightsModule::command = {};
bool LightsModule::publish_error = 0;
Logger LightsModule::logger = Logger("LightsModule");

LightsModule::LightsModule(){
    update_params();
    instance_initialized = true;
    duty_cycle = toggle_period * (duty_cycle_ptc/100.0);
    init();

}

void LightsModule::callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        for (uint8_t i = 0; i<raw_command.number_of_commands; i++){
            if (raw_command.commands[i].light_id == light_id){
                command = raw_command.commands[i];
                command_ptr = &command;
                break;
            }
        }

        publish_error = false;
    } else {
        publish_error = true;
    }
}

RgbSimpleColor LightsModule::change_color(RgbSimpleColor color) {
    int a = HAL_GetTick() % 15000;
    if (a==0) {
        color = RgbSimpleColor::RED_COLOR;
        logger.log_debug("Color: RED");
    } else if (a == 5000) {
        color = RgbSimpleColor::GREEN_COLOR;
        logger.log_debug("Color: GREEN");
    } else if (a==10000) {
        color = RgbSimpleColor::BLUE_COLOR;
        logger.log_debug("Color: BLUE");
    }
    return color;
}

void LightsModule::init() {

    int_led_driver = GPIORgbLedDriver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE);

    int_led_driver.toggle_period = toggle_period;
    int_led_driver.duty_cycle = duty_cycle;

    ext_led_driver = PwmRgbLedDriver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6);
    if (blink_type != 0){
        ext_led_driver.toggle_period = toggle_period;
        ext_led_driver.duty_cycle = duty_cycle;
    }

    ext_led_driver.set_intensity(max_intensity);
    
    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    if (sub_id < 0) {
        logger.log_debug("uavcanSubscribe failed");
    }

    RgbSimpleColor color_int = RgbSimpleColor::BLUE_COLOR;
    int_led_driver.reset();
    ext_led_driver.reset();
    int_led_driver.set(color_int);
    ext_led_driver.set(default_color);
}

void LightsModule::update_params(){
    paramsLoadFromFlash();

    auto blink_period = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);
    
    blink_type = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_TYPE);
    
    max_intensity = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    duty_cycle_ptc =  paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
    
    light_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHT_ID);

    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);
    RgbSimpleColor default_rgb_color = RgbSimpleColor(default_color);

    ext_led_driver.set(default_rgb_color);
}

void LightsModule::spin_once() {

    if (HAL_GetTick() % 10000 == 0) {
        update_params();  
    } 

    if (command_ptr != nullptr) {
        Rgb565Color color={};
        color = {
            command.color.red, 
            command.color.green, 
            command.color.blue
            };
        ext_led_driver.set(color);
    }

    if (blink_type == 2){
        auto intensity = ((HAL_GetTick()%1000)/ 1000.0) * (max_intensity);
        
        if(HAL_GetTick()%1000 ==0 ){
            sprintf(buffer, "intensity: %d", intensity);
            logger.log_debug(buffer);

        }  
        ext_led_driver.set_intensity(intensity);
    }

    int_led_driver.toggle();
    ext_led_driver.toggle();
}