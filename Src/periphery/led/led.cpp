/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "periphery/led/led.hpp"
#include "main.h"

// TODO: decide how to set duty_cycle (via init or constructor).
void PwmRgbLedDriver::init(uint16_t duty_cycle, uint16_t blink_period){}

PwmRgbLedDriver::PwmRgbLedDriver(){}

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

void PwmRgbLedDriver::toggle(){
    auto crnt_time_ms = HAL_GetTick();
    bool state = (crnt_time_ms % toggle_period > duty_cycle);
    if (state){
        reset();
        return;
    }
    
    char buffer[90];

    if (HAL_GetTick()%1000 ==0){
        sprintf(buffer, "%d, %d, %d", _current_rgb565_color.red, _current_rgb565_color.green, _current_rgb565_color.blue);
        logger.log_debug(buffer);
    }

    PwmPeriphery::set_duty_cycle_pct(red_pin, _current_rgb565_color.red);
    PwmPeriphery::set_duty_cycle_pct(green_pin, _current_rgb565_color.green);
    PwmPeriphery::set_duty_cycle_pct(blue_pin, _current_rgb565_color.blue);
}

void PwmRgbLedDriver::set(Rgb565Color color){
    _current_rgb565_color.red     = float(color.red * intensity) / (red_intensity_div * red_max);
    _current_rgb565_color.green   = float(color.green * intensity) / (green_intensity_div * green_max);
    _current_rgb565_color.blue    = float(color.blue * intensity) / (blue_intensity_div * blue_max);
}

void PwmRgbLedDriver::set(RgbSimpleColor color){
    uint8_t red     = 0;
    uint8_t green   = 0;
    uint8_t blue    = 0;
    
    switch (color)
    {
    case RgbSimpleColor::RED_COLOR:
        red = red_max;
        break;
    case RgbSimpleColor::GREEN_COLOR:
        green = green_max;
        break;

    case RgbSimpleColor::BLUE_COLOR:
        blue = blue_max;
        break;

    case RgbSimpleColor::CYAN_COLOR:
        green = green_max;
        blue = blue_max;
        break;
    
    case RgbSimpleColor::MAGENTA_COLOR:
        red = red_max;
        blue = blue_max;
        break;

    case RgbSimpleColor::YELLOW_COLOR:
        red = red_max;
        green = green_max;
        break;

    case RgbSimpleColor::WHITE_COLOR:
        red = red_max;
        green = green_max;
        blue = blue_max;
        break;

    default:
        break;
    }

    _current_rgb565_color.red     = float(red * intensity) / (red_intensity_div * red_max);
    _current_rgb565_color.green   = float(green * intensity) / (green_intensity_div * green_max);
    _current_rgb565_color.blue    = float(blue * intensity) / (blue_intensity_div * blue_max);
}
void PwmRgbLedDriver::set_intensity(uint8_t intensity){

    this->intensity = intensity;
}


void GPIORgbLedDriver::init(uint16_t duty_cycle, uint16_t blink_period){}

GPIORgbLedDriver::GPIORgbLedDriver(){}

GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin){
    red_pin = red_gpio_pin;
    green_pin = green_gpio_pin;
    blue_pin = blue_gpio_pin;
}

void GPIORgbLedDriver::set(Rgb565Color color){
    reset();
    _current_rgb565_color = color;
}

void GPIORgbLedDriver::set(RgbSimpleColor color){
    _current_rgb565_color = Rgb565Color{ 0, 0, 0};
    switch (color) {
        case RgbSimpleColor::RED_COLOR:
            _current_rgb565_color.red = 1;
            break;

        case RgbSimpleColor::GREEN_COLOR:
            _current_rgb565_color.green = 1;
            break;

        case RgbSimpleColor::BLUE_COLOR:
            _current_rgb565_color.blue = 1;
            break;

        case RgbSimpleColor::CYAN_COLOR:
            _current_rgb565_color.green = 1;
            _current_rgb565_color.blue = 1;
            break;
        
        case RgbSimpleColor::MAGENTA_COLOR:
            _current_rgb565_color.red = 1;
            _current_rgb565_color.blue = 1;
            break;

        case RgbSimpleColor::YELLOW_COLOR:
            _current_rgb565_color.red = 1;
            _current_rgb565_color.green = 1;
            break;

        case RgbSimpleColor::WHITE_COLOR:
            _current_rgb565_color.red = 1;
            _current_rgb565_color.green = 1;
            _current_rgb565_color.blue = 1;
            break;

        default:
            set(RgbSimpleColor::RED_COLOR);
            break;
        }
    set(_current_rgb565_color);
}

void GPIORgbLedDriver::reset(LedColor color){
    switch (color) {
    case LedColor::RED:
        _current_rgb565_color.red = 0;
        GPIOPeriphery::reset(red_pin);
        break;

    case LedColor::GREEN:
        _current_rgb565_color.green = 0;
        GPIOPeriphery::reset(green_pin);
        break;

    case LedColor::BLUE:
        _current_rgb565_color.blue = 0;
        GPIOPeriphery::reset(blue_pin);
        break;
    
    default:
        GPIOPeriphery::reset(red_pin);
        GPIOPeriphery::reset(green_pin);
        GPIOPeriphery::reset(blue_pin);
        break;
    }
    set(_current_rgb565_color);
}

void GPIORgbLedDriver::set_intensity(uint8_t intensity){}

void GPIORgbLedDriver::toggle(){
    reset();
    if (_current_rgb565_color.red > 0) GPIOPeriphery::toggle(red_pin, toggle_period, duty_cycle);
    if (_current_rgb565_color.green > 0) GPIOPeriphery::toggle(green_pin, toggle_period, duty_cycle);
    if (_current_rgb565_color.blue > 0) GPIOPeriphery::toggle(blue_pin, toggle_period, duty_cycle);
}
