/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/gpio/gpio.hpp"
#include "main.h"

Logger GPIOPeriphery::_logger = Logger("GPIO");


void GPIOPeriphery::set(GPIOPin gpio_pin){
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
GPIO_PinState GPIOPeriphery::get(GPIOPin gpio_pin){
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


void GPIOPeriphery::reset() {
    HAL_GPIO_WritePin(INT_RGB_LED_RED_GPIO_Port, INT_RGB_LED_RED_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INT_RGB_LED_GREEN_GPIO_Port, INT_RGB_LED_GREEN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_SET);
}


void GPIOPeriphery::toggle(GPIOPin gpio_pin, uint32_t blink_period, uint32_t duty_cycle){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % blink_period > duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
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
