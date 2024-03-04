/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "periphery/led/led.hpp"
#include "main.h"


Rgb565Color Rgb565Color::from_rgb_simple_color(RgbSimpleColor color) {
    switch (color) {
    case RgbSimpleColor::RED_COLOR:
        return Rgb565Color{31, 0, 0};

    case RgbSimpleColor::GREEN_COLOR:
        return Rgb565Color{0, 63, 0};

    case RgbSimpleColor::BLUE_COLOR:
        return Rgb565Color{0, 0, 31};

    case RgbSimpleColor::MAGENTA_COLOR:
        return Rgb565Color{31, 0, 31};

    case RgbSimpleColor::YELLOW_COLOR:
        return Rgb565Color{31, 63, 0};

    case RgbSimpleColor::CYAN_COLOR:
        return Rgb565Color{0, 63, 31};

    case RgbSimpleColor::WHITE_COLOR:
        return Rgb565Color{31, 63, 31};

    default:
        return Rgb565Color{0, 0, 0};
    }
}

void RgbLedInterface::set(Rgb565Color color) {
    _current_rgb565_color = color;
}

void RgbLedInterface::set(RgbSimpleColor color) {
    _current_rgb565_color.from_rgb_simple_color(color);
}

void RgbLedInterface::spin() {
    if (toggle_period_ms == 0) {
        _toggle_flag = false;
    } else {
        _toggle_flag = (HAL_GetTick() % toggle_period_ms) > duty_cycle_ms;
    }
}

void RgbLedInterface::reset(LedColor color) {
    if (color == LedColor::RED || color == LedColor::ALL) {
        _current_rgb565_color.red = 0;
    }
    if (color == LedColor::GREEN || color == LedColor::ALL) {
        _current_rgb565_color.green = 0;
    }
    if (color == LedColor::BLUE || color == LedColor::ALL) {
        _current_rgb565_color.blue = 0;
    }
}

void PwmRgbLedDriver::init() {
    PwmPeriphery::init(red_pin);
    PwmPeriphery::init(green_pin);
    PwmPeriphery::init(blue_pin);
}

PwmRgbLedDriver::PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin) {
        red_pin=red_pwm_pin;
        green_pin=green_pwm_pin;
        blue_pin=blue_pwm_pin;

        red_max = 31;
        green_max = 63;
        blue_max = 31;
        init();
};

void PwmRgbLedDriver::reset(LedColor color) {
    RgbLedInterface::reset(color);
    set(_current_rgb565_color);
}

void PwmRgbLedDriver::set() {
    red_ticks = float(_current_rgb565_color.red * intensity) / (red_intensity_div * red_max);
    green_ticks = float(_current_rgb565_color.green * intensity) / (green_intensity_div * green_max);
    blue_ticks = float(_current_rgb565_color.blue * intensity) / (green_intensity_div * green_max);

    PwmPeriphery::set_duty_cycle_pct(red_pin, red_ticks);
    PwmPeriphery::set_duty_cycle_pct(green_pin, green_ticks);
    PwmPeriphery::set_duty_cycle_pct(blue_pin, blue_ticks);
}

void PwmRgbLedDriver::set(Rgb565Color color) {
    RgbLedInterface::set(color);
    set();
}

void PwmRgbLedDriver::set(RgbSimpleColor color) {
    RgbLedInterface::set(color);
    set();
}

void PwmRgbLedDriver::set_intensity(uint8_t intensity_val) {
    if (intensity_val > 100) {
        intensity_val = 100;
    }

    intensity = intensity_val;
    
    red_ticks = float(_current_rgb565_color.red * intensity) / (red_intensity_div * red_max);
    green_ticks = float(_current_rgb565_color.green * intensity) / (green_intensity_div * green_max);
    blue_ticks = float(_current_rgb565_color.blue * intensity) / (green_intensity_div * green_max);
}

void PwmRgbLedDriver::spin(Rgb565Color color) {
    RgbLedInterface::spin();
    set(_toggle_flag ? Rgb565Color{0,0,0} : color);
}

GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin) {
    red_pin = red_gpio_pin;
    green_pin = green_gpio_pin;
    blue_pin = blue_gpio_pin;
    red_max = 1;
    green_max = 1;
    blue_max = 1;
}

void GPIORgbLedDriver::reset(LedColor color) {
    RgbLedInterface::reset(color);
    set(_current_rgb565_color);
}

void GPIORgbLedDriver::set() {
    if (_current_rgb565_color.red)  GPIOPeripheryInverted::set(red_pin);
    else GPIOPeripheryInverted::reset(red_pin);
    if (_current_rgb565_color.green) GPIOPeripheryInverted::set(green_pin);
    else GPIOPeripheryInverted::reset(green_pin);
    if (_current_rgb565_color.blue) GPIOPeripheryInverted::set(blue_pin);
    else GPIOPeripheryInverted::reset(blue_pin);
}

void GPIORgbLedDriver::set(RgbSimpleColor color) {
    RgbLedInterface::set(color);
    set();
}

void GPIORgbLedDriver::set(Rgb565Color color) {
    RgbLedInterface::set(color);
    set();
}

void GPIORgbLedDriver::spin(Rgb565Color color) {
    RgbLedInterface::spin();
    set(_toggle_flag ? Rgb565Color{0,0,0} : color);
}