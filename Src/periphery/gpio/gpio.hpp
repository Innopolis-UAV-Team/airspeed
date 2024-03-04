/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#ifndef SRC_PERIPHERY_GPIO_HPP_
#define SRC_PERIPHERY_GPIO_HPP_

#include <stdint.h>
#include "logger.hpp"

#ifdef __cplusplus
extern "C" {
#endif

enum class GPIOPin {
    INT_RGB_LED_RED,    //
    INT_RGB_LED_GREEN,  //
    INT_RGB_LED_BLUE,   //
};

class GPIOPeripheryInverted {
    static Logger _logger;
public:
    static void set(const GPIOPin gpio_pin);
    static GPIO_PinState get(const GPIOPin gpio_pin);
    static void toggle(GPIOPin gpio_pin, uint32_t toggle_period_ms, uint32_t duty_cycle_ms);
    static void reset(GPIOPin gpio_pin);
    static void reset();
};

#ifdef __cplusplus
}
#endif

#endif  // SRC_PERIPHERY_GPIO_HPP_