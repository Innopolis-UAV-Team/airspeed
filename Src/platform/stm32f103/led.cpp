/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"
#include "main.h"


LedGPIOPins int_gpio_led_pins = {
    #ifdef INT_RGB_LED_RED_Pin
    .pin_red = INT_RGB_LED_RED_Pin,
    #endif 
    .pin_green = INT_RGB_LED_GREEN_Pin, 
    .pin_blue = INT_RGB_LED_BLUE_Pin,
    #ifdef INT_RGB_LED_RED_Pin
    .gpio_port_red = INT_RGB_LED_RED_GPIO_Port,
    #endif
    .gpio_port_green = INT_RGB_LED_GREEN_GPIO_Port,
    .gpio_port_blue = INT_RGB_LED_BLUE_GPIO_Port,
};

LedPWMPins int_pwm_led_pins = {
    .pwm_pin_red = PwmPin::PWM_2,
    .pwm_pin_green = PwmPin::PWM_1,
};

LedGPIOPins ext_gpio_led_pins = {
    .pin_red = EXT_RGB_LED_RED_Pin, 
    .pin_green = EXT_RGB_LED_GREEN_Pin, 
    .pin_blue = EXT_RGB_LED_BLUE_Pin,

    .gpio_port_red = EXT_RGB_LED_RED_GPIO_Port,
    .gpio_port_green = EXT_RGB_LED_GREEN_GPIO_Port,
    .gpio_port_blue = EXT_RGB_LED_BLUE_GPIO_Port,
};

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

void LedPorts::reset(LedPinColor color){
    switch (color) {
    case RED:
        HAL_GPIO_WritePin(gpio_pins.gpio_port_red, gpio_pins.pin_red, GPIO_PIN_SET);
        break;

    case GREEN:
        HAL_GPIO_WritePin(gpio_pins.gpio_port_green, gpio_pins.pin_green, GPIO_PIN_SET);
        break;

    case BLUE:
        HAL_GPIO_WritePin(gpio_pins.gpio_port_blue, gpio_pins.pin_blue, GPIO_PIN_SET);
        break;

    case ALL:
        HAL_GPIO_WritePin(gpio_pins.gpio_port_red, gpio_pins.pin_red, GPIO_PIN_SET);
        HAL_GPIO_WritePin(gpio_pins.gpio_port_green, gpio_pins.pin_green, GPIO_PIN_SET);
        HAL_GPIO_WritePin(gpio_pins.gpio_port_blue, gpio_pins.pin_blue, GPIO_PIN_SET);
        break;

    default:
        break;
    }
}

void LedPorts::toggle(GPIO_PinState states [3]){
    HAL_GPIO_WritePin(gpio_pins.gpio_port_red, gpio_pins.pin_red, states[0]);
    HAL_GPIO_WritePin(gpio_pins.gpio_port_green, gpio_pins.pin_green, states[1]);
    HAL_GPIO_WritePin(gpio_pins.gpio_port_blue, gpio_pins.pin_blue, states[2]);
    if (states[0]==GPIO_PIN_RESET){
        PwmPeriphery::reset(pwm_pins.pwm_pin_red);
    } else {
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_red, led_conf.duty_cycle);
    }
    if (states[1]==GPIO_PIN_RESET){
        PwmPeriphery::reset(pwm_pins.pwm_pin_green);
    } else {
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_green, led_conf.duty_cycle);
    }
    if (states[2]==GPIO_PIN_RESET){
        PwmPeriphery::reset(pwm_pins.pwm_pin_blue);
    } else {
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_blue, led_conf.duty_cycle);
    }
}

void LedPorts::set(LedPinColor color, uint8_t intensity){
    switch (color) {
    case RED:
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_red, intensity);
        break;
    case GREEN:
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_green, intensity);
        break;
    case BLUE:
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_blue, intensity);
        break;
    case ALL:
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_red, intensity);
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_green, intensity);
        PwmPeriphery::set_duration(pwm_pins.pwm_pin_blue, intensity);
    default:
        break;
    }
}

void LedPeriphery::reset_internal(LedPinColor pin_color){
    led_conf.int_led_pin_out.reset(pin_color);
}
void LedPeriphery::reset_external(LedPinColor pin_color){
    led_conf.ext_led_pin_out.reset(pin_color);
}

void LedPeriphery::set_internal(uint8_t intensity){
    led_conf.int_led_pin_out.set(LedPinColor::ALL, intensity);
    char buffer[90];
    #ifndef NDEBUG
    sprintf(buffer, "TIM1->ARR, TIM1->CCR1: %d %d", TIM1->ARR, TIM1->CCR1);
    led_conf._led_logger.log_debug(buffer);
    sprintf(buffer, "TIM2->ARR, TIM2->CCR1: %d %d", TIM2->ARR, TIM2->CCR1);
    led_conf._led_logger.log_debug(buffer);
    #endif
}

void LedPeriphery::set_external(uint8_t intensity){
    led_conf.int_led_pin_out.set(LedPinColor::ALL, intensity);
}

void LedPeriphery::toggle_external(LedColor color){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState states [3] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};
    switch (color)
    {
    case LedColor::RED_COLOR:
        states[0] = state;
        break;
    case LedColor::GREEN_COLOR:
        states[1] = state;
        break;
    case LedColor::BLUE_COLOR:
        states[2] = state;
        break;
    case LedColor::CYAN_COLOR:
        states[1] = state;
        states[2] = state;
        break;

    case LedColor::MAGENTA_COLOR:
        states[0] = state;
        states[2] = state;
        break;

    case LedColor::YELLOW_COLOR:
        states[0] = state;
        states[1] = state;
        break;

    case LedColor::WHITE_COLOR:
        states[0] = state;
        states[1] = state;
        states[2] = state;
        break;
    default:
        return;
    }
    led_conf.ext_led_pin_out.toggle(states);

}


void LedPeriphery::toggle_internal(LedColor color){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState states [3] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};
    switch (color)
    {
    case LedColor::RED_COLOR:
        states[0] = state;
        break;
    case LedColor::GREEN_COLOR:
        states[1] = state;
        break;
    case LedColor::BLUE_COLOR:
        states[2] = state;
        break;
    case LedColor::CYAN_COLOR:
        states[1] = state;
        states[2] = state;
        break;

    case LedColor::MAGENTA_COLOR:
        states[0] = state;
        states[2] = state;
        break;

    case LedColor::YELLOW_COLOR:
        states[0] = state;
        states[1] = state;
        break;

    case LedColor::WHITE_COLOR:
        states[0] = state;
        states[1] = state;
        states[2] = state;
        break;
    default:
        return;
    }
    led_conf.int_led_pin_out.toggle(states);

}

void LedPeriphery::set_duty_cycle(float duty_cycle_fraction){
    led_conf.duty_cycle_ptc = duty_cycle_fraction * 100;
    led_conf.duty_cycle = led_conf.blink_period * duty_cycle_fraction;
}

void LedPeriphery::set_blink_period(uint16_t period){
    led_conf.blink_period = period;
    set_duty_cycle(led_conf.duty_cycle_ptc/100.0);
}

//  TODO: delete state and color from the set(), and just use toggle gpio version instead to set a color. 
//  TODO: delete versions of LedPorts classes, since they are identical.