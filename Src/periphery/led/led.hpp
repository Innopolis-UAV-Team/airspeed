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

struct Rgb565Color {
    uint8_t red     : 5;
    uint8_t green   : 6;
    uint8_t blue    : 5;

    Rgb565Color parseRgb565Color(uint8_t red_val, uint8_t green_val, uint8_t blue_val){
        red = red_val;
        green = green_val;
        blue = blue_val;
        return *this;
    }
};
// Rgb565Color Rgb565Color::Rgb565Color(uint8_t red_val, uint8_t green_val, uint8_t blue_val){

// }
enum class RgbSimpleColor {
    RED_COLOR,
    GREEN_COLOR,
    BLUE_COLOR,
    CYAN_COLOR,
    MAGENTA_COLOR,
    YELLOW_COLOR,
    WHITE_COLOR,
    COLORS_AMOUNT = 7,
};

enum LedColor{
    RED,
    GREEN,
    BLUE,
    ALL
};


class RgbLedInterface {
public:
    uint16_t duty_cycle = 0;
    uint16_t blink_period = 0;

    virtual void init(uint16_t duty_cycle, uint16_t blink_period) = 0;

    virtual void set(RgbSimpleColor color) = 0;
    virtual void set(Rgb565Color color) = 0;
    virtual void set_intensity(uint8_t intensity) =0;
    virtual void toggle() =0;
    virtual void reset(LedColor color = LedColor::ALL) = 0;
};

class PwmRgbLedDriver: public RgbLedInterface{
private:
    PwmPin red_pin;
    PwmPin green_pin;
    PwmPin blue_pin;

    uint8_t red_val;
    uint8_t green_val;
    uint8_t blue_val;

    uint8_t intensity = 100;
    uint8_t red_intensity_div = 2;
    uint8_t green_intensity_div = 1;
    uint8_t blue_intensity_div = 1;

    uint8_t red_max = 31;
    uint8_t green_max = 63;
    uint8_t blue_max = 31;
    Logger logger = Logger("PwmRgbLedDriver");
    void init(uint16_t duty_cycle, uint16_t blink_period) override;
    
public:
    PwmRgbLedDriver();
    PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin);
    void set(RgbSimpleColor color) override;
    void set(Rgb565Color color) override;
    void set_intensity(uint8_t intensity) override;
    void toggle() override;
    void reset(LedColor color = LedColor::ALL) override;
};

class GPIORgbLedDriver: public RgbLedInterface {
private:
    GPIOPin red_pin;
    GPIOPin green_pin;
    GPIOPin blue_pin;

    uint8_t red_val;
    uint8_t green_val;
    uint8_t blue_val;
    void set_intensity(uint8_t intensity) override;
    void init(uint16_t duty_cycle, uint16_t blink_period) override;
public:
    GPIORgbLedDriver();
    GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin);

    void set(RgbSimpleColor color) override;
    void set(Rgb565Color color) override;
    void toggle() override;
    void reset(LedColor color = LedColor::ALL) override;
};

class Ws2812Driver: public RgbLedInterface {
private:
    PwmPin pwm_pin;
    uint16_t num_of_leds;
public:

    Ws2812Driver( PwmPin pwm_pin, uint16_t num_of_leds);
    void set(RgbSimpleColor color) override;
    void set(Rgb565Color color) override;
    void set_intensity(uint8_t intensity) override;
    void toggle() override;
    void reset(LedColor color = LedColor::ALL) override;
};

#endif  // SRC_PERIPHERY_LED_LED_HPP_