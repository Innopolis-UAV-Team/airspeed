/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "periphery/led/led.hpp"
#include "main.h"


void RgbLedInterface::set(Rgb565Color color){
    _current_rgb565_color = color;
}

void RgbLedInterface::set(RgbSimpleColor color){
    switch (color)
    {
    case RgbSimpleColor::RED_COLOR:
        _current_rgb565_color = Rgb565Color{red_max, 0, 0};
        break;
    case RgbSimpleColor::GREEN_COLOR:
        _current_rgb565_color = Rgb565Color{0, green_max, 0};
        break;

    case RgbSimpleColor::BLUE_COLOR:
        _current_rgb565_color = Rgb565Color{0, 0, blue_max};
        break;

    case RgbSimpleColor::CYAN_COLOR:
        _current_rgb565_color = Rgb565Color{0, green_max, blue_max};
        break;
    
    case RgbSimpleColor::MAGENTA_COLOR:
        _current_rgb565_color = Rgb565Color{red_max, 0, blue_max};
        break;

    case RgbSimpleColor::YELLOW_COLOR:
        _current_rgb565_color = Rgb565Color{red_max, green_max, 0};
        break;

    case RgbSimpleColor::WHITE_COLOR:
        _current_rgb565_color = Rgb565Color{red_max, green_max, blue_max};
        break;

    default:
        _current_rgb565_color = Rgb565Color{0, 0, 0};
        break;
    }
}

void RgbLedInterface::reset(LedColor color){
    switch (color) {
    case LedColor::RED:
        _current_rgb565_color.red = 0;
        break;
        
    case LedColor::GREEN:
        _current_rgb565_color.green = 0;
        break;

    case LedColor::BLUE:
        _current_rgb565_color.blue = 0;
        break;
    
    default:
        _current_rgb565_color = Rgb565Color{0, 0, 0};
        break;
    }
}

void PwmRgbLedDriver::init(){
    PwmPeriphery::init(red_pin);
    PwmPeriphery::init(green_pin);
    PwmPeriphery::init(blue_pin);
}

PwmRgbLedDriver::PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin){
        red_pin=red_pwm_pin;
        green_pin=green_pwm_pin;
        blue_pin=blue_pwm_pin;

        red_max = 31;
        green_max = 63;
        blue_max = 31;
        init();
};

void PwmRgbLedDriver::reset(LedColor color){
    RgbLedInterface::reset(color);
    switch (color) {
    case LedColor::RED:
        _current_rgb565_color.red = 0;
        PwmPeriphery::reset(red_pin);
        break;
        
    case LedColor::GREEN:
        _current_rgb565_color.green = 0;
        PwmPeriphery::reset(green_pin);
        break;

    case LedColor::BLUE:
        _current_rgb565_color.blue = 0;
        PwmPeriphery::reset(blue_pin);
        break;
    
    default:
        PwmPeriphery::reset(red_pin);
        PwmPeriphery::reset(green_pin);
        PwmPeriphery::reset(blue_pin);
        _current_rgb565_color = Rgb565Color{0,0,0};
        break;
    }
    set(_current_rgb565_color);
}

void PwmRgbLedDriver::set(Rgb565Color color){
    RgbLedInterface::set(color);

    red_ticks = float(_current_rgb565_color.red * intensity) / (red_intensity_div * red_max);
    green_ticks = float(_current_rgb565_color.green * intensity) / (green_intensity_div * green_max);
    blue_ticks = float(_current_rgb565_color.blue * intensity) / (green_intensity_div * green_max);
}

void PwmRgbLedDriver::set(RgbSimpleColor color){
    RgbLedInterface::set(color);

    red_ticks = float(_current_rgb565_color.red * intensity) / (red_intensity_div * red_max);
    green_ticks = float(_current_rgb565_color.green * intensity) / (green_intensity_div * green_max);
    blue_ticks = float(_current_rgb565_color.blue * intensity) / (green_intensity_div * green_max);
}

void PwmRgbLedDriver::spin(){
    auto crnt_time_ms = HAL_GetTick();
    bool state = (crnt_time_ms % toggle_period_ms > duty_cycle_ms);
    if (state) {
        PwmPeriphery::reset(red_pin);
        PwmPeriphery::reset(green_pin);
        PwmPeriphery::reset(blue_pin);
        return;
    }
    
    char buffer[90];

    if (HAL_GetTick()%500 ==0){
        sprintf(buffer, "%d, %d, %d", red_ticks, green_ticks, blue_ticks);
        logger.log_debug(buffer);
    }

    PwmPeriphery::set_duty_cycle_pct(red_pin, red_ticks);
    PwmPeriphery::set_duty_cycle_pct(green_pin, green_ticks);
    PwmPeriphery::set_duty_cycle_pct(blue_pin, blue_ticks);
}

void PwmRgbLedDriver::set_intensity(uint8_t intensity_val){
    if (intensity_val > 100) {
        intensity_val = 100;
    }

    intensity = intensity_val;
    
    red_ticks = float(_current_rgb565_color.red * intensity) / (red_intensity_div * red_max);
    green_ticks = float(_current_rgb565_color.green * intensity) / (green_intensity_div * green_max);
    blue_ticks = float(_current_rgb565_color.blue * intensity) / (green_intensity_div * green_max);
}

GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin){
    red_pin = red_gpio_pin;
    green_pin = green_gpio_pin;
    blue_pin = blue_gpio_pin;
    red_max = 1;
    green_max = 1;
    blue_max = 1;
}

void GPIORgbLedDriver::reset(LedColor color){
    RgbLedInterface::reset(color);

    switch (color) {
    case LedColor::RED:
        GPIOPeripheryInverted::reset(red_pin);
        break;

    case LedColor::GREEN:
        GPIOPeripheryInverted::reset(green_pin);
        break;

    case LedColor::BLUE:
        GPIOPeripheryInverted::reset(blue_pin);
        break;
    
    default:
        GPIOPeripheryInverted::reset(red_pin);
        GPIOPeripheryInverted::reset(green_pin);
        GPIOPeripheryInverted::reset(blue_pin);
        break;
    }
    set(_current_rgb565_color);
}

void GPIORgbLedDriver::spin(){
    GPIOPeripheryInverted::reset();
    if (_current_rgb565_color.red > 0) GPIOPeripheryInverted::toggle(red_pin, toggle_period_ms, duty_cycle_ms);
    if (_current_rgb565_color.green > 0) GPIOPeripheryInverted::toggle(green_pin, toggle_period_ms, duty_cycle_ms);
    if (_current_rgb565_color.blue > 0) GPIOPeripheryInverted::toggle(blue_pin, toggle_period_ms, duty_cycle_ms);
}