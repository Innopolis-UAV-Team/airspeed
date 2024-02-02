/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"


LedGPIOPins int_gpio_led_pins = {};

LedPWMPins int_pwm_led_pins = {
    .pwm_pin_red = PwmPin::PWM_2,
    .pwm_pin_green = PwmPin::PWM_1,
};

LedGPIOPins ext_gpio_led_pins = {};

LedPWMPins ext_pwm_led_pins = {
    .pwm_pin_red = PwmPin::PWM_4,
    .pwm_pin_green = PwmPin::PWM_5,
    .pwm_pin_blue = PwmPin::PWM_3,
};


LedPorts::LedPorts(bool is_internal){
    if (is_internal){
        pwm_pins=int_pwm_led_pins;
        gpio_pins=int_gpio_led_pins;
    } else {
        pwm_pins=ext_pwm_led_pins;
        gpio_pins=ext_gpio_led_pins;
    }
}

LedData led_conf = {.int_led_pin_out = LedPorts(true), .ext_led_pin_out=LedPorts(false)};


void LedPorts::reset(LedPinColor color){}
void LedPorts::set(LedPinColor color, uint8_t intensity_ptc){}
void LedPeriphery::reset_internal(LedPinColor pin_color){}
void LedPeriphery::reset_external(LedPinColor pin_color){}
void LedPeriphery::set_internal(uint8_t intensity_ptc){}
void LedPeriphery::set_external(uint8_t intensity_ptc){}
void LedPeriphery::toggle_external(LedColor color){}
void LedPeriphery::toggle_internal(LedColor color){}
void LedPeriphery::set_duty_cycle(float duty_cycle_fraction){}
void LedPeriphery::set_blink_period(uint32_t period){}