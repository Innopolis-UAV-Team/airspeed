/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "periphery/gpio/gpio.hpp"
#include "main.h"

void GPIOPeripheryInverted::set(GPIOPin gpio_pin){
    switch (gpio_pin) {
        case GPIOPin::INT_RGB_LED_RED :
            HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, GPIO_PIN_RESET);
            break;

        case GPIOPin::INT_RGB_LED_GREEN :
            HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, GPIO_PIN_RESET);
            break;

        case GPIOPin::INT_RGB_LED_BLUE :
            HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_RESET);
            break;
        default:
            break;
    }
}
GPIO_PinState GPIOPeripheryInverted::get(GPIOPin gpio_pin){
    switch (gpio_pin) {
        case GPIOPin::INT_RGB_LED_RED :
            return HAL_GPIO_ReadPin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin);

        case GPIOPin::INT_RGB_LED_GREEN :
            return HAL_GPIO_ReadPin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin);

        case GPIOPin::INT_RGB_LED_BLUE :
            return HAL_GPIO_ReadPin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin);
        default:
        return GPIO_PIN_SET;
    }
}


void GPIOPeripheryInverted::reset() {
    HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_SET);
}

void GPIOPeripheryInverted::reset(GPIOPin gpio_pin) {
    switch (gpio_pin) {
        case GPIOPin::INT_RGB_LED_RED :
            HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, GPIO_PIN_SET);
            break;
        case GPIOPin::INT_RGB_LED_GREEN :
            HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, GPIO_PIN_SET);
            break;
        case GPIOPin::INT_RGB_LED_BLUE :
            HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}


void GPIOPeripheryInverted::toggle(GPIOPin gpio_pin, uint32_t toggle_period_ms, uint32_t duty_cycle_ms){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % toggle_period_ms > duty_cycle_ms) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    switch (gpio_pin) {
        case GPIOPin::INT_RGB_LED_RED :
            HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, state);
            break;

        case GPIOPin::INT_RGB_LED_GREEN :
            HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, state);
            break;

        case GPIOPin::INT_RGB_LED_BLUE :
            HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, state);
            break;

        default:
            break;
    }
}
