/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"
#include "main.h"

LedData led_conf;


PwmPin LedPortsINT::port_red = PwmPin::PWM_1;
GPIO_TypeDef* LedPortsINT::port_green = INT_RGB_LED_GREEN_GPIO_Port;
GPIO_TypeDef* LedPortsINT::port_blue = INT_RGB_LED_BLUE_GPIO_Port;

uint16_t LedPortsINT::pin_red = 0;
uint16_t LedPortsINT::pin_green = INT_RGB_LED_GREEN_Pin;
uint16_t LedPortsINT::pin_blue = INT_RGB_LED_BLUE_Pin;

void LedPortsINT::reset_red(){
    PwmPeriphery::reset(port_red);
}

void LedPortsINT::reset_green(){
    HAL_GPIO_WritePin(port_green, pin_green, GPIO_PIN_SET);
}

void LedPortsINT::reset_blue(){
    HAL_GPIO_WritePin(port_blue, pin_blue, GPIO_PIN_SET);
}

void LedPortsINT::reset(LedPinColor color){
    switch (color) {
    case RED:
        reset_red();
        break;
    case GREEN:
        reset_green();
        break;
    case BLUE:
        reset_blue();
        break;
    case ALL:
        reset_red();
        reset_green();
        reset_blue();
    default:
        break;
    }
}

void LedPortsINT::set_red(uint8_t intensity, GPIO_PinState state){
    PwmPeriphery::set_duty_cycle(port_red, intensity);
}

void LedPortsINT::set_green(uint8_t intensity, GPIO_PinState state){
    HAL_GPIO_WritePin(port_green, pin_green, state);
}

void LedPortsINT::set_blue(uint8_t intensity, GPIO_PinState state){
    HAL_GPIO_WritePin(port_blue, pin_blue, state);
}

void LedPortsINT::set(LedPinColor color, uint8_t intensity, GPIO_PinState state){
    switch (color) {
    case RED:
        set_red(intensity, state);
        break;
    case GREEN:
        set_green(intensity, state);
        break;
    case BLUE:
        set_blue(intensity, state);
        break;
    case ALL:
        set_red(intensity, state);
        set_green(intensity, state);
        set_blue(intensity, state);
    default:
        break;
    }
}
#ifdef EXT_RGB_LED_RED_GPIO_Port

GPIO_TypeDef* LedPortsEXT::port_red = EXT_RGB_LED_RED_GPIO_Port;
GPIO_TypeDef* LedPortsEXT::port_green = EXT_RGB_LED_GREEN_GPIO_Port;
GPIO_TypeDef* LedPortsEXT::port_blue = EXT_RGB_LED_BLUE_GPIO_Port;

uint16_t LedPortsEXT::pin_red = EXT_RGB_LED_RED_Pin;
uint16_t LedPortsEXT::pin_green = EXT_RGB_LED_GREEN_Pin;
uint16_t LedPortsEXT::pin_blue = EXT_RGB_LED_BLUE_Pin;


void LedPortsEXT::reset(LedPinColor color){
    switch (color) {
    case RED:
        HAL_GPIO_WritePin(port_red, pin_red, GPIO_PIN_SET);
        break;

    case GREEN:
        HAL_GPIO_WritePin(port_green, pin_green, GPIO_PIN_SET);
        break;

    case BLUE:
        HAL_GPIO_WritePin(port_blue, pin_blue, GPIO_PIN_SET);
        break;

    case ALL:
        HAL_GPIO_WritePin(port_red, pin_red, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port_green, pin_green, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port_blue, pin_blue, GPIO_PIN_SET);

    default:
        break;
    }
}
void LedPortsEXT::set(LedPinColor color, uint8_t intensity, GPIO_PinState state){
    switch (color) {
    
    case RED:
        HAL_GPIO_WritePin(port_red, pin_red, state);
        break;

    case GREEN:
        HAL_GPIO_WritePin(port_green, pin_green, state);
        break;

    case BLUE:
        HAL_GPIO_WritePin(port_blue, pin_blue, state);
        break;

    case ALL:
        HAL_GPIO_WritePin(port_red, pin_red, state);
        HAL_GPIO_WritePin(port_green, pin_green, state);
        HAL_GPIO_WritePin(port_blue, pin_blue, state);

    default:
        break;
    }
}
#else

PwmPin LedPortsEXT::port_red = PwmPin::PWM_4;
PwmPin LedPortsEXT::port_green = PwmPin::PWM_5;
PwmPin LedPortsEXT::port_blue = PwmPin::PWM_3;


void LedPortsEXT::reset(LedPinColor color){
    switch (color) {
    case RED:
        PwmPeriphery::reset(port_red);
        break;

    case GREEN:
        PwmPeriphery::reset(port_green);
        break;

    case BLUE:
        PwmPeriphery::reset(port_blue);
        break;

    case ALL:
        PwmPeriphery::reset(port_red);
        PwmPeriphery::reset(port_green);
        PwmPeriphery::reset(port_blue);

    default:
        break;
    }
}
void LedPortsEXT::set(LedPinColor color, uint8_t intensity, GPIO_PinState state){
    switch (color) {
    
    case RED:
        PwmPeriphery::set_duty_cycle(port_red, intensity);
        break;

    case GREEN:
        PwmPeriphery::set_duty_cycle(port_green, intensity);
        break;

    case BLUE:
        PwmPeriphery::set_duty_cycle(port_blue, intensity);
        break;

    case ALL:
        PwmPeriphery::set_duty_cycle(port_red, intensity);
        PwmPeriphery::set_duty_cycle(port_green, intensity);
        PwmPeriphery::set_duty_cycle(port_blue, intensity);

    default:
        break;
    }
}

#endif

void LedPeriphery::reset_internal(LedPinColor pin_color){
    led_conf.int_led_pin_out.reset(pin_color);
}
void LedPeriphery::reset_external(LedPinColor pin_color){
    led_conf.ext_led_pin_out.reset(pin_color);
}

void LedPeriphery::set_internal(LedColor color, uint8_t intensity){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    reset_internal(LedPinColor::ALL);
    switch (color) {
    case LedColor::RED_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, intensity, state);
        break;

    case LedColor::GREEN_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        break;

    case LedColor::BLUE_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;

    case LedColor::CYAN_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;

    case LedColor::MAGENTA_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, intensity, state);
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;

    case LedColor::YELLOW_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, intensity, state);
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        break;

    case LedColor::WHITE_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::ALL, intensity, state);
        break;
    default:
        return;
    }
}

void LedPeriphery::set_external(LedColor color, uint8_t intensity){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    reset_external(LedPinColor::ALL);
    switch (color)
    {
    case LedColor::RED_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, intensity, state);
        break;
    case LedColor::GREEN_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        break;
    case LedColor::BLUE_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;
    case LedColor::CYAN_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;

    case LedColor::MAGENTA_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, intensity, state);
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, intensity, state);
        break;

    case LedColor::YELLOW_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, intensity, state);
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, intensity, state);
        break;

    case LedColor::WHITE_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::ALL, intensity, state);
        break;
    default:
        return;
    }
}