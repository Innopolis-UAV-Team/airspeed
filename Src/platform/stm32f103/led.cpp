/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"
#include "main.h"

PwmRgbLedDriver::PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin){
        red_pin=red_pwm_pin;
        green_pin=green_pwm_pin;
        blue_pin=blue_pwm_pin;
        PwmPeriphery::init(red_pwm_pin);
        PwmPeriphery::init(green_pwm_pin);
        PwmPeriphery::init(blue_pwm_pin);
};

void PwmRgbLedDriver::reset(LedColor color){
    switch (color)
    {
    case LedColor::RED:
        PwmPeriphery::reset(red_pin);
        break;
        
    case LedColor::GREEN:
        PwmPeriphery::reset(green_pin);
        break;

    case LedColor::BLUE:
        PwmPeriphery::reset(blue_pin);
        break;
    
    default:
        PwmPeriphery::reset(red_pin);
        PwmPeriphery::reset(green_pin);
        PwmPeriphery::reset(blue_pin);
        break;
    }
}

void PwmRgbLedDriver::toggle(){
    auto crnt_time_ms = HAL_GetTick();
    bool state = (crnt_time_ms % blink_period > duty_cycle);
    if (state){
        reset();
        return;
    }

    uint8_t red     = float(red_val * intensity) / (red_max);
    uint8_t green   = float(green_val* intensity) / (green_max);
    uint8_t blue    = float(blue_val * intensity) / (blue_max);

    PwmPeriphery::set_duty_cycle_pct(red_pin, red);
    PwmPeriphery::set_duty_cycle_pct(green_pin, green);
    PwmPeriphery::set_duty_cycle_pct(blue_pin, blue);
}

void PwmRgbLedDriver::set(Rgb565Color color){
    red_val = color.red;
    green_val = color.green;
    blue_val = color.blue;
}

void PwmRgbLedDriver::set(RgbSimpleColor color){
    switch (color)
    {
    case RgbSimpleColor::RED_COLOR:
        red_val = red_max;
        green_val = 0;
        blue_val = 0;
        break;
    case RgbSimpleColor::GREEN_COLOR:
        red_val = 0;
        green_val = green_max;
        blue_val = 0;
    case RgbSimpleColor::BLUE_COLOR:
        red_val = 0;
        green_val = 0;
        blue_val = blue_max;
    case RgbSimpleColor::CYAN_COLOR:
        red_val = 0;
        green_val = green_max;
        blue_val = blue_max;
    
    case RgbSimpleColor::MAGENTA_COLOR:
        red_val = red_max;
        green_val = 0;
        blue_val = blue_max;
    case RgbSimpleColor::YELLOW_COLOR:
        red_val = red_max;
        green_val = green_max;
        blue_val = 0;
    case RgbSimpleColor::WHITE_COLOR:
        red_val = red_max;
        green_val = green_max;
        blue_val = blue_max;
    default:
        break;
    }
}

GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin){
    red_pin = red_gpio_pin;
    green_pin = green_gpio_pin;
    blue_pin = blue_gpio_pin;
}
