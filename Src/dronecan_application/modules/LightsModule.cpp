/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "LightsModule.hpp"

// Initialize the static member variables
char buffer[90];
uint8_t LightsModule::light_id = 0;
uint8_t LightsModule::blink_type = 0;
LightsModule LightsModule::instance = LightsModule();
bool LightsModule::instance_initialized = false;

SingleLightCommand_t* LightsModule::command_ptr = nullptr;
SingleLightCommand_t LightsModule::command = {};
bool LightsModule::publish_error = 0;
Logger LightsModule::logger = Logger("LightsModule");


LightsModule &LightsModule::get_instance() {
    if (!instance_initialized) {
        if (HAL_GetTick()%1000 ==0){
            logger.log_debug("instance not initialized!");
        }
    }

    return instance;
}

LightsModule &LightsModule::get_instance(uint8_t duty_cycle_ptc_val, uint16_t blink_period_val, uint8_t max_intensity_val, RgbSimpleColor default_color_val, uint8_t light_id_val,  uint8_t blink_type_val) {
    instance_initialized = true;
    instance.blink_period = blink_period_val;
    instance.duty_cycle_ptc = duty_cycle_ptc_val;
    instance.max_intensity = max_intensity_val;
    instance.duty_cycle = instance.blink_period * (instance.duty_cycle_ptc/100.0);
    instance.default_color = default_color_val;
    light_id = light_id_val;
    blink_type = blink_type_val;
    instance.init();

    logger.log_debug("instance init");
    
    return instance;
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

    int_led_driver.blink_period = blink_period;
    int_led_driver.duty_cycle = duty_cycle;

    ext_led_driver = PwmRgbLedDriver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6);
    if (blink_type != 0){
        ext_led_driver.blink_period = blink_period;
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
    ext_led_driver.set(instance.default_color);
}

void LightsModule::spin_once() {
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