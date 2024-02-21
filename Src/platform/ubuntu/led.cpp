/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"

void PwmRgbLedDriver::init(uint16_t duty_cycle, uint16_t blink_period){}
PwmRgbLedDriver::PwmRgbLedDriver(){}
PwmRgbLedDriver::PwmRgbLedDriver(PwmPin red_pwm_pin, PwmPin green_pwm_pin, PwmPin blue_pwm_pin){}
void PwmRgbLedDriver::reset(LedColor color){}
void PwmRgbLedDriver::toggle(){}
void PwmRgbLedDriver::set(Rgb565Color color){}
void PwmRgbLedDriver::set(RgbSimpleColor color){}
void PwmRgbLedDriver::set_intensity(uint8_t intensity){}
void GPIORgbLedDriver::init(uint16_t duty_cycle, uint16_t blink_period){}
GPIORgbLedDriver::GPIORgbLedDriver(){}
GPIORgbLedDriver::GPIORgbLedDriver(GPIOPin red_gpio_pin, GPIOPin green_gpio_pin, GPIOPin blue_gpio_pin){}
void GPIORgbLedDriver::set(Rgb565Color color){}
void GPIORgbLedDriver::set(RgbSimpleColor color){}
void GPIORgbLedDriver::reset(LedColor color){}
void GPIORgbLedDriver::set_intensity(uint8_t intensity){}
void GPIORgbLedDriver::toggle(){}