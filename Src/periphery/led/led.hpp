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
    COLORS_AMOUNT,
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
    virtual void set(LedPinColor color, uint16_t intensity) = 0;
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

    GPIO_TypeDef* gpio_port_red;
    GPIO_TypeDef* gpio_port_green;
    GPIO_TypeDef* gpio_port_blue;
};

class LedPorts: public LedInterface{
public:
    LedPWMPins pwm_pins={};
    LedGPIOPins gpio_pins={};

    LedPorts(bool is_internal = true);
    void set(LedPinColor color, uint16_t intensity) override;
    void reset(LedPinColor color= LedPinColor::ALL) override;
    void toggle(GPIO_PinState states [3]) override;
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
    void set(LedPinColor color, uint16_t intensity) override;
    void reset(LedPinColor color = LedPinColor::ALL) override;
    void toggle(GPIO_PinState states [3]) override;
};

#endif

struct LedData
{
    Logger _led_logger = Logger("LED");

    LedPorts int_led_pin_out;
    LedPorts ext_led_pin_out;

    uint16_t max_int_intensity;
    uint16_t max_ext_intensity;

    uint16_t blink_period = 1000;
    float duty_cycle_ptc = 100; // 0.5 * 1000
    uint16_t duty_cycle = 1000;

    LedColor int_current_color;
    LedColor ext_current_color;
};


namespace LedPeriphery{

    // internal
    // toggle_internal()
    // set_internal()

    // external

    void toggle_internal(LedColor color);
    void toggle_external(LedColor color);

    void set_internal(uint16_t intensity);
    void set_external(uint16_t intensity);

    void reset_internal(LedPinColor pin_color=LedPinColor::ALL);
    void reset_external(LedPinColor pin_color=LedPinColor::ALL);
    
    void set_intensity(uint16_t intensity, bool to_internal);
    void set_duty_cycle(float duty_cycle_fraction);
    void set_blink_period(uint16_t period);
};


#endif  // SRC_APPLICATION_PERIPHERY_LED_LED_HPP_
