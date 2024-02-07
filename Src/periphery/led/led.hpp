/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef SRC_APPLICATION_PERIPHERY_LED_HPP_
#define SRC_APPLICATION_PERIPHERY_LED_HPP_

#include "main.h"
#include <stdint.h>
#include "periphery/pwm/pwm.hpp"
#include "logger.hpp"

enum class LedColor {
    RED_COLOR,
    GREEN_COLOR,
    BLUE_COLOR,
    CYAN_COLOR,
    MAGENTA_COLOR,
    YELLOW_COLOR,
    WHITE_COLOR,
    COLORS_AMOUNT = 7,
};

enum LedPinColor{
    RED,
    GREEN,
    BLUE,
    ALL
};


class LedInterface {
public:
    virtual void toggle(GPIO_PinState states [3])=0;
    virtual void set(LedPinColor color, uint8_t intensity_ptc) = 0;
    virtual void reset(LedPinColor color) = 0;
};

struct LedPWMPins {
    PwmPin pwm_pin_red;
    PwmPin pwm_pin_green;
    PwmPin pwm_pin_blue;

};

#ifdef STM32F103xB
struct LedGPIOPins {
    uint16_t pin_red;
    uint16_t pin_green;
    uint16_t pin_blue;

    GPIO_TypeDef* gpio_port_red = nullptr;
    GPIO_TypeDef* gpio_port_green = nullptr;
    GPIO_TypeDef* gpio_port_blue = nullptr;
};

class LedPorts: public LedInterface{
public:
    LedPWMPins pwm_pins={};
    LedGPIOPins gpio_pins={};

    LedPorts(bool is_internal = true);
    void set(LedPinColor color, uint8_t intensity_ptc) override;
    void reset(LedPinColor color= LedPinColor::ALL) override;
    void toggle(GPIO_PinState states [3]){};
};

#else
struct LedGPIOPins {
    uint16_t pin_red;
    uint16_t pin_green;
    uint16_t pin_blue;

    uint16_t gpio_port_red;
    uint16_t gpio_port_green;
    uint16_t gpio_port_blue;
};

class LedPorts : public LedInterface{
public:
    LedPWMPins pwm_pins={};
    LedGPIOPins gpio_pins={};

    LedPorts(bool is_internal = true);
    void set(LedPinColor color, uint8_t intensity_ptc) override;
    void reset(LedPinColor color = LedPinColor::ALL) override;
    void toggle(GPIO_PinState states [3]){};
};

#endif

struct LedData
{
    Logger _led_logger = Logger("LED");

    LedPorts int_led_pin_out;
    LedPorts ext_led_pin_out;

    uint8_t max_int_intensity_ptc;
    uint8_t max_ext_intensity_ptc;

    uint8_t max_red = 31;
    uint8_t max_green = 63;
    uint8_t max_blue = 31;

    uint32_t blink_period = 1000;
    uint8_t duty_cycle_ptc = 100;
    uint32_t duty_cycle = 1000;

    LedColor int_current_color;
    LedColor ext_current_color;
};


namespace LedPeriphery{

    void toggle_internal(LedColor color);
    void toggle_external(LedColor color);

    void toggle_rgb_internal(uint8_t red, uint8_t green, uint8_t blue);
    void toggle_rgb_external(uint8_t red, uint8_t green, uint8_t blue);

    void set_internal(uint8_t intensity_ptc);
    void set_external(uint8_t intensity_ptc);

    void reset_internal(LedPinColor pin_color=LedPinColor::ALL);
    void reset_external(LedPinColor pin_color=LedPinColor::ALL);
    
    void set_intensity(uint8_t intensity_ptc, bool to_internal);
    void set_duty_cycle_pct(float duty_cycle_fraction);
    void set_blink_period(uint32_t period);
};


#endif  // SRC_APPLICATION_PERIPHERY_LED_LED_HPP_
