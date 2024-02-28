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
bool LightsModule::is_cmd_received = false;

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
                is_cmd_received = true;
                break;
            }
        }
        publish_error = false;
    } else {
        publish_error = true;
    }
}

RgbSimpleColor LightsModule::change_color(RgbSimpleColor color) {
    color = static_cast<RgbSimpleColor>(static_cast<int>(color) + 1);

    switch (color)
    {
    case RgbSimpleColor::RED_COLOR:
        logger.log_debug("Color: RED");
        break;
    case RgbSimpleColor::GREEN_COLOR:
        logger.log_debug("Color: GREEN");
        break;
    case RgbSimpleColor::BLUE_COLOR:
        logger.log_debug("Color: BLUE");
        break;
    case RgbSimpleColor::YELLOW_COLOR:
        logger.log_debug("Color: YELLOW");
        break;
    case RgbSimpleColor::CYAN_COLOR:
        logger.log_debug("Color: CYAN");
    case RgbSimpleColor::MAGENTA_COLOR:
        logger.log_debug("Color: MAGENTA");
        break;
    case RgbSimpleColor::WHITE_COLOR:
        logger.log_debug("Color: WHITE");
        break;
    default:
        color = RgbSimpleColor::RED_COLOR;
        break;
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
    ext_led_driver.set(_current_color);
}

void LightsModule::update_params(){
    paramsLoadFromFlash();

    auto blink_period = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);
    
    blink_type = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_TYPE);
    
    max_intensity = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    duty_cycle_ptc =  paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
    
    light_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHT_ID);

    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);
    _current_color = RgbSimpleColor(default_color);

    ext_led_driver.set(_current_color);
}

void LightsModule::spin_once() {

    if (HAL_GetTick() % 10000 == 0){
        update_params();  
    } 

    if (is_cmd_received) {
        Rgb565Color color={};
        color = {
            command.color.red, 
            command.color.green, 
            command.color.blue
            };
        ext_led_driver.set(color);
    }

    if (blink_type == 2){
        auto intensity = ((HAL_GetTick() % toggle_period) / (float)toggle_period) * (max_intensity);
        
        if(HAL_GetTick()%1000 ==0 ){
            sprintf(buffer, "intensity: %d", intensity);
            logger.log_debug(buffer);

        }  
        ext_led_driver.set_intensity(intensity);
    }

    int_led_driver.toggle();
    ext_led_driver.toggle();
}