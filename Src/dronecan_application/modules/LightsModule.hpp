
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
    static LightsModule * entity;
    uint8_t duty_cycle_ptc = 100;
    uint16_t blink_period = 0;
    uint16_t duty_cycle = 0;
    uint8_t max_intensity = 0;
    RgbSimpleColor default_color;
    static bool publish_error;
    static Logger logger;
    
    static void callback(CanardRxTransfer* transfer);
    void init();
protected:
    LightsModule(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity,RgbSimpleColor default_color);
public:
    static LightsCommand_t command;
    static LightsCommand_t* command_ptr;
    GPIORgbLedDriver int_led_driver;
    PwmRgbLedDriver ext_led_driver;

    LightsModule(LightsModule &other) = delete;
    void operator=(const LightsModule &) = delete;
    static LightsModule *GetInstance(uint8_t duty_cycle_ptc, uint16_t blink_period, uint8_t max_intensity, RgbSimpleColor default_color);
    static LightsModule *GetInstance();
    void spin_once();
    void reset_command();
    RgbSimpleColor change_color(RgbSimpleColor color);
};
#endif //SRC_MODULE_LIGHTS_HPP_