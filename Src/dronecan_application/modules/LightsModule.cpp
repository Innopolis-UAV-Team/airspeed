#include "params.hpp"
#include "main.h"
#include "dronecan.h"
#include "logger.hpp"

#include "periphery/led/led.hpp"
#include "uavcan/equipment/indication/LightsCommand.h"

extern LedData led_conf;
LightsCommand_t command = {};
bool publish_error = false;
Logger logger = Logger("LED");
LedColor color_int = LedColor::BLUE_COLOR;

void callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        command = raw_command;
    } else {
        publish_error = true;
    }
}


LedColor change_color(LedColor color){
    int a = HAL_GetTick() % 15000;
    if (a == 5000){
        color = LedColor::BLUE_COLOR;
        led_conf._led_logger.log_debug("Color: BLUE");
    } else if (a==10000) {
        color = LedColor::GREEN_COLOR;
        led_conf._led_logger.log_debug("Color: GREEN");

    } else if (a==0) {
        color = LedColor::RED_COLOR;
        led_conf._led_logger.log_debug("Color: RED");
    }
    return color;
}

void init(){
    led_conf._led_logger = Logger("LED");

    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);
    auto blink_period = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);

    uint8_t max_ext_intensity_ptc = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    led_conf.duty_cycle_ptc =  paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
    
    LedPeriphery::update_ext_intensity(max_ext_intensity_ptc);
    LedPeriphery::set_blink_period(blink_period);

    led_conf.max_int_intensity_ptc = 100;

    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    if (sub_id < 0) {
        logger.log_error("sub_id < 0");
    }

    color_int = LedColor::BLUE_COLOR;
    LedColor color_ext = LedColor(default_color);

    LedPeriphery::reset_internal();
    LedPeriphery::reset_external();

    LedPeriphery::toggle_external(color_ext);
    LedPeriphery::toggle_internal(color_int);
}

void spin_once(LedColor int_color){
    
    LedPeriphery::toggle_internal(int_color);

}

void reset(){
    command.commands[0].color = {.red=0, .green=0,.blue=0};
}