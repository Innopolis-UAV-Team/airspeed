#include "LightsModule.hpp"

LightsModule* LightsModule::entity = nullptr;
LightsCommand_t* LightsModule::command_ptr = nullptr;
LightsCommand_t LightsModule::command = {};
bool LightsModule::publish_error = 0;
Logger LightsModule::logger = Logger("LightsModule");

LightsModule::LightsModule(uint8_t duty_cycle_ptc_val, uint16_t blink_period_val, uint8_t max_intensity_val, RgbSimpleColor default_color_val){
    entity = this;
    blink_period = blink_period_val;
    duty_cycle_ptc = duty_cycle_ptc_val;
    max_intensity = max_intensity_val;
    duty_cycle = blink_period * (duty_cycle_ptc/100.0);
    default_color = default_color;
    init();
}

LightsModule *LightsModule::GetInstance(){
    if (entity == nullptr){
        logger.log_debug("LightsModule is not");
    }
    return entity;
}

LightsModule *LightsModule::GetInstance(uint8_t duty_cycle_ptc_val, uint16_t blink_period_val, uint8_t max_intensity_val, RgbSimpleColor default_color_val)
{
    if (entity == nullptr) {
        entity = new LightsModule(duty_cycle_ptc_val, blink_period_val, max_intensity_val, default_color_val);
    }
    return entity;
}

void LightsModule::callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        command = raw_command;
        publish_error = false;
        command_ptr = &raw_command;
    } else {
        publish_error = true;
    }
}

RgbSimpleColor LightsModule::change_color(RgbSimpleColor color){
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

void LightsModule::init(){

    int_led_driver = GPIORgbLedDriver(GPIOPin::INT_RGB_LED_RED, GPIOPin::INT_RGB_LED_GREEN, GPIOPin::INT_RGB_LED_BLUE);

    int_led_driver.blink_period = blink_period;
    int_led_driver.duty_cycle = duty_cycle;

    ext_led_driver = PwmRgbLedDriver(PwmPin::PWM_4, PwmPin::PWM_3, PwmPin::PWM_6);

    ext_led_driver.blink_period = blink_period;
    ext_led_driver.duty_cycle = duty_cycle;
    ext_led_driver.set_intensity(max_intensity);
    
    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    if (sub_id < 0) {
        logger.log_error("sub_id < 0");
    }

    RgbSimpleColor color_int = RgbSimpleColor::BLUE_COLOR;
    int_led_driver.reset();
    ext_led_driver.reset();
    int_led_driver.set(color_int);
    ext_led_driver.set(default_color);
}

void LightsModule::spin_once(){
    if (command_ptr != nullptr) {
        Rgb565Color color={};
        color.parseRgb565Color(command.commands->color.red, command.commands->color.green, command.commands->color.blue);
        ext_led_driver.set(color);
    }
    

    int_led_driver.toggle();
    ext_led_driver.toggle();
}