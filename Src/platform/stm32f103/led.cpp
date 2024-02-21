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
    
    // logger.log_debug("toggle");

    uint8_t red     = float(red_val * intensity) / (red_intensity_div * red_max);
    uint8_t green   = float(green_val * intensity) / (green_intensity_div * green_max);
    uint8_t blue    = float(blue_val * intensity) / (blue_intensity_div * blue_max);

    // uint8_t red     = float(red_val * intensity ) / (red_max);
    // uint8_t green   = float(green_val * intensity) / (green_max);
    // uint8_t blue    = float(blue_val * intensity) / (blue_max);
    
    char buffer[90];

    if (HAL_GetTick()%1000 ==0){
        sprintf(buffer, "%d, %d, %d", red, green, blue);
        logger.log_debug(buffer);
    }
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
        break;

    case RgbSimpleColor::BLUE_COLOR:
        red_val = 0;
        green_val = 0;
        blue_val = blue_max;
        break;

    case RgbSimpleColor::CYAN_COLOR:
        red_val = 0;
        green_val = green_max;
        blue_val = blue_max;
        break;
    
    case RgbSimpleColor::MAGENTA_COLOR:
        red_val = red_max;
        green_val = 0;
        blue_val = blue_max;
        break;

    case RgbSimpleColor::YELLOW_COLOR:
        red_val = red_max;
        green_val = green_max;
        blue_val = 0;
        break;

    case RgbSimpleColor::WHITE_COLOR:
        red_val = red_max;
        green_val = green_max;
        blue_val = blue_max;
        break;

    default:
        break;
    }
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
    if (color.red >0){
        red_val = 1;
    }
    if (color.green >0){
        green_val = 1;
    }
    if (color.blue >0){
        blue_val = 1;
    }
}

void GPIORgbLedDriver::set(RgbSimpleColor color){
    switch (color) {
        case RgbSimpleColor::RED_COLOR:
            red_val = 1;
            green_val = 0;
            blue_val = 0;
            break;

        case RgbSimpleColor::GREEN_COLOR:
            red_val = 0;
            green_val = 1;
            blue_val = 0;
            break;

        case RgbSimpleColor::BLUE_COLOR:
            red_val = 0;
            green_val = 0;
            blue_val = 1;
            break;

        case RgbSimpleColor::CYAN_COLOR:
            red_val = 0;
            green_val = 1;
            blue_val = 1;
            break;
        
        case RgbSimpleColor::MAGENTA_COLOR:
            red_val = 1;
            green_val = 0;
            blue_val = 1;
            break;

        case RgbSimpleColor::YELLOW_COLOR:
            red_val = 1;
            green_val = 1;
            blue_val = 0;
            break;

        case RgbSimpleColor::WHITE_COLOR:
            red_val = 1;
            green_val = 1;
            blue_val = 1;
            break;

        default:
            set(RgbSimpleColor::RED_COLOR);
            break;
    }
}

void GPIORgbLedDriver::reset(LedColor color){
    switch (color) {
    case LedColor::RED:
        red_val = 0;
        GPIOPeriphery::reset(red_pin);
        break;

    case LedColor::GREEN:
        green_val = 0;
        GPIOPeriphery::reset(green_pin);
        break;

    case LedColor::BLUE:
        blue_val = 0;
        GPIOPeriphery::reset(blue_pin);
        break;
    
    default:
        GPIOPeriphery::reset(red_pin);
        GPIOPeriphery::reset(green_pin);
        GPIOPeriphery::reset(blue_pin);
        break;
    }
}

void GPIORgbLedDriver::set_intensity(uint8_t intensity){}

void GPIORgbLedDriver::toggle(){
    reset();
    if (red_val > 0) GPIOPeriphery::toggle(red_pin, blink_period, duty_cycle);
    if (green_val > 0) GPIOPeriphery::toggle(green_pin, blink_period, duty_cycle);
    if (blue_val > 0) GPIOPeriphery::toggle(blue_pin, blink_period, duty_cycle);
}
