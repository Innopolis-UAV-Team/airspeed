/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 


#include "periphery/gpio/gpio.hpp"
#include "main.h"

Logger GPIOPeriphery::_logger = Logger("GPIO");

void GPIOPeriphery::set(GPIOPin gpio_pin){}
GPIO_PinState GPIOPeriphery::get(GPIOPin gpio_pin){
    return GPIO_PIN_RESET;
}
void GPIOPeriphery::reset() {}
void GPIOPeriphery::reset(GPIOPin gpio_pin) {}
void GPIOPeriphery::toggle(GPIOPin gpio_pin, uint32_t blink_period, uint32_t duty_cycle){}