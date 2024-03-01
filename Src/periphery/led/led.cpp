/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "periphery/led/led.hpp"
#include "main.h"

<<<<<<< HEAD
// TODO: decide how to set duty_cycle (via init or constructor).
void PwmRgbLedDriver::init(uint16_t duty_cycle, uint16_t blink_period){}
=======

void RgbLedInterface::set(Rgb565Color color){
    _current_rgb565_color = color;
}
>>>>>>> 37a65a2 (apply suggestions)

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
<<<<<<< HEAD
    bool state = (crnt_time_ms % toggle_period > duty_cycle);
    if (state){
=======
    bool state = (crnt_time_ms % toggle_period_ms > duty_cycle_ms);
    if (state) {
>>>>>>> 37a65a2 (apply suggestions)
        reset();
        return;
    }
    
    char buffer[90];

    if (HAL_GetTick()%1000 ==0){
<<<<<<< HEAD
        sprintf(buffer, "%d, %d, %d", red_val, green_val, blue_val);
        logger.log_debug(buffer);
    }

    PwmPeriphery::set_duty_cycle_pct(red_pin, red_val);
    PwmPeriphery::set_duty_cycle_pct(green_pin, green_val);
    PwmPeriphery::set_duty_cycle_pct(blue_pin, blue_val);
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
=======
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
>>>>>>> 37a65a2 (apply suggestions)
}

GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin){
    red_pin = red_gpio_pin;
    green_pin = green_gpio_pin;
    blue_pin = blue_gpio_pin;
<<<<<<< HEAD
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
=======
    red_max = 1;
    green_max = 1;
    blue_max = 1;
>>>>>>> 37a65a2 (apply suggestions)
}

void GPIORgbLedDriver::reset(LedColor color){
    RgbLedInterface::reset(color);

    switch (color) {
    case LedColor::RED:
<<<<<<< HEAD
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
=======
        GPIOPeripheryInverted::reset(red_pin);
        break;

    case LedColor::GREEN:
        GPIOPeripheryInverted::reset(green_pin);
        break;

    case LedColor::BLUE:
        GPIOPeripheryInverted::reset(blue_pin);
>>>>>>> 37a65a2 (apply suggestions)
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
    reset();
<<<<<<< HEAD
    if (_current_rgb565_color.red > 0) GPIOPeriphery::toggle(red_pin, toggle_period, duty_cycle);
    if (_current_rgb565_color.green > 0) GPIOPeriphery::toggle(green_pin, toggle_period, duty_cycle);
    if (_current_rgb565_color.blue > 0) GPIOPeriphery::toggle(blue_pin, toggle_period, duty_cycle);
=======
    if (_current_rgb565_color.red > 0) GPIOPeripheryInverted::toggle(red_pin, toggle_period_ms, duty_cycle_ms);
    if (_current_rgb565_color.green > 0) GPIOPeripheryInverted::toggle(green_pin, toggle_period_ms, duty_cycle_ms);
    if (_current_rgb565_color.blue > 0) GPIOPeripheryInverted::toggle(blue_pin, toggle_period_ms, duty_cycle_ms);
>>>>>>> 37a65a2 (apply suggestions)
}
