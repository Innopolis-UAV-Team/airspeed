/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef SRC_APPLICATION_PERIPHERY_LED_HPP_
#define SRC_APPLICATION_PERIPHERY_LED_HPP_

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
};

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

    uint8_t red_max = 31;
    uint8_t green_max = 63;
    uint8_t blue_max = 31;
public:

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
public:
    GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin);

    void set(RgbSimpleColor color) override;
    void set(Rgb565Color color) override;
    void set_intensity(uint8_t intensity) override;
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

// struct LedData
// {
//     Logger _led_logger = Logger("LED");

//     LedPorts int_led_pin_out;
//     LedPorts ext_led_pin_out;

//     uint8_t max_int_intensity_ptc;
//     uint8_t max_ext_intensity_ptc;

//     uint8_t max_red = 31;
//     uint8_t max_green = 63;
//     uint8_t max_blue = 31;

//     uint32_t blink_period = 1000;
//     uint8_t duty_cycle_ptc = 100;
//     uint32_t duty_cycle = 1000;

//     LedColor int_current_color;
//     LedColor ext_current_color;

//     uint8_t red_intensity;
//     uint8_t blue_intensity;
//     uint8_t green_intensity;
// };


// namespace LedPeriphery{

//     void toggle_internal(LedColor color);
//     void toggle_external(LedColor color);

//     void toggle_rgb_internal(uint8_t red, uint8_t green, uint8_t blue);
//     void toggle_rgb_external(uint8_t red, uint8_t green, uint8_t blue);

//     void reset_internal(RgbSimpleColor pin_color=RgbSimpleColor::ALL);
//     void reset_external(RgbSimpleColor pin_color=RgbSimpleColor::ALL);
    
//     void set_intensity(uint8_t intensity_ptc, bool to_internal);
//     void set_duty_cycle_pct(float duty_cycle_fraction);
//     void set_blink_period(uint32_t period);
//     void update_ext_intensity(uint8_t max_intensity);
// };


#endif  // SRC_APPLICATION_PERIPHERY_LED_LED_HPP_
