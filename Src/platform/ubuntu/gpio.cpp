/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file
 *LICENSE.
 ***/

#include "periphery/gpio/gpio.hpp"

#include "main.h"

Logger GPIOPeripheryInverted::_logger = Logger("GPIO");

void GPIOPeripheryInverted::set(GPIOPin gpio_pin) {}
GPIO_PinState GPIOPeripheryInverted::get(GPIOPin gpio_pin) {
    return GPIO_PIN_RESET;
}
void GPIOPeripheryInverted::reset() {}
void GPIOPeripheryInverted::reset(GPIOPin gpio_pin) {}
void GPIOPeripheryInverted::toggle(GPIOPin gpio_pin, uint32_t toggle_period_ms,
                                   uint32_t duty_cycle_ms) {}
