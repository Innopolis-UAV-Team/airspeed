/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "LightsModule.hpp"

uint8_t LightsModule::light_id = 0;
uint8_t LightsModule::toggle_type = 0;
LightsModule LightsModule::instance = LightsModule();
bool LightsModule::instance_initialized = false;

bool LightsModule::is_cmd_received = false;
SingleLightCommand_t LightsModule::command = {};
Logger LightsModule::logger = Logger("LightsModule");


LightsModule::LightsModule(): int_led_driver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE),  ext_led_driver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6){
    update_params();
    instance.init();
    instance_initialized = true;
};


LightsModule &LightsModule::get_instance() {
    return instance;
}


void LightsModule::reset_command(){
    command = {};
}


void LightsModule::spin_once() {
    Rgb565Color color;
    if (is_cmd_received) {
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

    int_led_driver.spin(Rgb565Color{0, 0, 1});
    ext_led_driver.spin(_current_color);

    if (verbose) {
        publish_command();
    }
}

void LightsModule::publish_command(){
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
    return color;
}


void LightsModule::update_params(){
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
    _current_color = Rgb565Color::from_rgb_simple_color(RgbSimpleColor(default_color));
    verbose = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_VERBOSE);
    char buffer[90];
    sprintf(buffer, "%d", verbose);
    logger.log_debug(buffer);
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