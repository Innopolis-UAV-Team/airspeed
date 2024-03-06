/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#ifndef SRC_PERIPHERY_LED_HPP_
#define SRC_PERIPHERY_LED_HPP_

#include "main.h"
#include <stdint.h>
#include "periphery/pwm/pwm.hpp"
#include "periphery/gpio/gpio.hpp"
#include "uavcan/equipment/indication/RGB565.h"

#include "logger.hpp"


enum class RgbSimpleColor {
    RED_COLOR = 0,
    GREEN_COLOR,
    BLUE_COLOR,
    MAGENTA_COLOR,
    YELLOW_COLOR,
    CYAN_COLOR,
    WHITE_COLOR,
    COLORS_AMOUNT = 7,
};

struct Rgb565Color {
    uint8_t red     : 5;
    uint8_t green   : 6;
    uint8_t blue    : 5;
    static Rgb565Color from_rgb_simple_color(RgbSimpleColor color);
};

enum LedColor {
    RED,
    GREEN,
    BLUE,
    ALL
};


class RgbLedInterface {
public:
    bool _toggle_flag = false;
    uint8_t red_max;
    uint8_t green_max;
    uint8_t blue_max;

    uint16_t duty_cycle_ms;
    uint16_t toggle_period_ms;
    Rgb565Color _current_rgb565_color;

    virtual void set(RgbSimpleColor color);
    virtual void set(Rgb565Color color);
    virtual void apply() = 0;
    virtual void spin(Rgb565Color color);
    virtual void reset(LedColor color = LedColor::ALL);
};

class PwmRgbLedDriver: public RgbLedInterface {
private:
    PwmPin red_pin;
    PwmPin green_pin;
    PwmPin blue_pin;

    uint8_t intensity = 100;
    uint8_t red_intensity_div = 1;
    uint8_t green_intensity_div = 1;
    uint8_t blue_intensity_div = 1;

    uint16_t red_ticks;
    uint16_t green_ticks;
    uint16_t blue_ticks;

    Logger logger = Logger("PwmRgbLedDriver");
    void apply();
    void init();

public:

    PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin);
    using RgbLedInterface::set;
    using RgbLedInterface::reset;
    void set_intensity(uint8_t intensity);
};

class GPIORgbLedDriver: public RgbLedInterface {
private:
    GPIOPin red_pin;
    GPIOPin green_pin;
    GPIOPin blue_pin;
    Logger logger = Logger("GPIORgbLedDriver");
    void apply() override;

public:
    GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin);
    using RgbLedInterface::set;
    using RgbLedInterface::reset;
};

class Ws2812Driver: public RgbLedInterface {
private:
    PwmPin pwm_pin;
    uint16_t num_of_leds;
    Logger logger = Logger("Ws2812Driver");
    void apply() override;
public:

    Ws2812Driver(PwmPin pwm_pin, uint16_t num_of_leds);
    void set_intensity(uint8_t intensity);
};

#endif  // SRC_PERIPHERY_LED_LED_HPP_