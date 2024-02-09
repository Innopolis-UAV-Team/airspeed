/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

LedGPIOPins int_gpio_led_pins = {
    #ifdef INT_RGB_LED_RED_Pin
    // .pin_red = INT_RGB_LED_RED_Pin,
    #endif 
    #ifdef INT_RGB_LED_GREEN_Pin
    // .pin_green = INT_RGB_LED_GREEN_Pin, 
    #endif
    .pin_blue = INT_RGB_LED_BLUE_Pin,
    #ifdef INT_RGB_LED_RED_Pin
    // .gpio_port_red = INT_RGB_LED_RED_GPIO_Port,
    #endif
    #ifdef INT_RGB_LED_GREEN_Pin
    // .gpio_port_green = INT_RGB_LED_GREEN_GPIO_Port,
    #endif
    .gpio_port_blue = INT_RGB_LED_BLUE_GPIO_Port,
};

LedPWMPins int_pwm_led_pins = {
    .pwm_pin_red = PwmPin::PWM_2,
    .pwm_pin_green = PwmPin::PWM_1,
};

LedGPIOPins ext_gpio_led_pins = {
//     #ifdef EXT_RGB_LED_RED_Pin
//     .pin_red = EXT_RGB_LED_RED_Pin,
//     #endif
//     #ifdef EXT_RGB_LED_GREEN_Pin
//     .pin_green = EXT_RGB_LED_GREEN_Pin, 
//     #endif
//     #ifdef EXT_RGB_LED_BLUE_Pin
//     .pin_blue = EXT_RGB_LED_BLUE_Pin,
//     #endif
//     #ifdef EXT_RGB_LED_RED_Pin
//     .gpio_port_red = EXT_RGB_LED_RED_GPIO_Port,
//     #endif
//     #ifdef EXT_RGB_LED_GREEN_Pin

//     .gpio_port_green = EXT_RGB_LED_GREEN_GPIO_Port,
//     #endif
//     #ifdef EXT_RGB_LED_BLUE_Pin
//     .gpio_port_blue = EXT_RGB_LED_BLUE_GPIO_Port,
//     #endif
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
        gpio_pins={};
    }
}

LedData led_conf = {.int_led_pin_out = LedPorts(true), .ext_led_pin_out=LedPorts(false)};

void LedPorts::reset(LedPinColor color){
    switch (color) {
    case RED:
        if (gpio_pins.gpio_port_red != nullptr){
            led_conf._led_logger.log_debug("RED");
            HAL_GPIO_WritePin(gpio_pins.gpio_port_red, gpio_pins.pin_red, GPIO_PIN_SET);
        } else {
            PwmPeriphery::reset(pwm_pins.pwm_pin_red);
        }
        break;

    case GREEN:
        if (gpio_pins.gpio_port_green != nullptr){
            led_conf._led_logger.log_debug("GREEN");
            HAL_GPIO_WritePin(gpio_pins.gpio_port_green, gpio_pins.pin_green, GPIO_PIN_SET);
        } else {
            PwmPeriphery::reset(pwm_pins.pwm_pin_green);
        }
        break;

    case BLUE:
        if (gpio_pins.gpio_port_blue != nullptr){
            HAL_GPIO_WritePin(gpio_pins.gpio_port_blue, gpio_pins.pin_blue, GPIO_PIN_SET);
            led_conf._led_logger.log_debug("BLUE");
        } else {
            PwmPeriphery::reset(pwm_pins.pwm_pin_blue);
        }
        break;
    case ALL:
        LedPorts::reset(LedPinColor::RED);
        LedPorts::reset(LedPinColor::GREEN);
        LedPorts::reset(LedPinColor::BLUE);
        break;

    default:
        break;
    }
}

void LedPorts::set(LedPinColor color, uint8_t intensity){
    switch (color) {
    case RED:
        if (gpio_pins.gpio_port_red != nullptr){
            led_conf._led_logger.log_debug("GPIO RED");
            HAL_GPIO_WritePin(gpio_pins.gpio_port_red, gpio_pins.pin_red, GPIO_PIN_RESET);
        } else {
            PwmPeriphery::set_duty_cycle_pct(pwm_pins.pwm_pin_red, intensity);
        }
        break;

    case GREEN:
        if (gpio_pins.gpio_port_green != nullptr){
            HAL_GPIO_WritePin(gpio_pins.gpio_port_green, gpio_pins.pin_green, GPIO_PIN_RESET);
            led_conf._led_logger.log_debug("GPIO GREEN");
        } else {
            PwmPeriphery::set_duty_cycle_pct(pwm_pins.pwm_pin_green, intensity);
        }
        break;

    case BLUE:
        if (gpio_pins.gpio_port_blue != nullptr){
            HAL_GPIO_WritePin(gpio_pins.gpio_port_blue, gpio_pins.pin_blue, GPIO_PIN_RESET);
        } else {
            led_conf._led_logger.log_debug("PWM BLUE");
            PwmPeriphery::set_duty_cycle_pct(pwm_pins.pwm_pin_blue, intensity);
        }
        break;

    case ALL:
        set(LedPinColor::RED, intensity);
        set(LedPinColor::BLUE, intensity);
        set(LedPinColor::GREEN, intensity);
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
    
}

void LedPeriphery::set_external(uint8_t intensity){
    led_conf.ext_led_pin_out.set(LedPinColor::ALL, intensity);
}

void LedPeriphery::toggle_external(LedColor color){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (state==GPIO_PIN_SET){
        led_conf.ext_led_pin_out.reset(LedPinColor::ALL);
        return;
    }
    
    switch (color) {
    case LedColor::RED_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.ext_led_pin_out.reset(LedPinColor::BLUE);
        break;
    case LedColor::GREEN_COLOR:
        led_conf.ext_led_pin_out.reset(LedPinColor::RED);
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.reset(LedPinColor::BLUE);
        break;
    case LedColor::BLUE_COLOR:
        led_conf.ext_led_pin_out.reset(LedPinColor::RED);
        led_conf.ext_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, led_conf.max_ext_intensity_ptc);
        break;
    case LedColor::CYAN_COLOR:
        led_conf.ext_led_pin_out.reset(LedPinColor::RED);
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, led_conf.max_ext_intensity_ptc);
        break;

    case LedColor::MAGENTA_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, led_conf.max_ext_intensity_ptc);
        break;

    case LedColor::YELLOW_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::RED, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, led_conf.max_ext_intensity_ptc);
        led_conf.ext_led_pin_out.reset(LedPinColor::BLUE);
        break;

    case LedColor::WHITE_COLOR:
        led_conf.ext_led_pin_out.set(LedPinColor::ALL, led_conf.max_ext_intensity_ptc);
        break;
    default:
        return;
    }
}


void LedPeriphery::toggle_internal(LedColor color){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (state==GPIO_PIN_SET){
        led_conf.int_led_pin_out.reset(LedPinColor::ALL);
        return;
    }
    switch (color) {
    case LedColor::RED_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.int_led_pin_out.reset(LedPinColor::BLUE);
        break;
    case LedColor::GREEN_COLOR:
        led_conf.int_led_pin_out.reset(LedPinColor::RED);
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.reset(LedPinColor::BLUE);
        break;
    case LedColor::BLUE_COLOR:
        led_conf.int_led_pin_out.reset(LedPinColor::RED);
        led_conf.int_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, led_conf.max_int_intensity_ptc);
        break;
    case LedColor::CYAN_COLOR:
        led_conf.int_led_pin_out.reset(LedPinColor::RED);
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, led_conf.max_int_intensity_ptc);
        break;

    case LedColor::MAGENTA_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.reset(LedPinColor::GREEN);
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, led_conf.max_int_intensity_ptc);
        break;

    case LedColor::YELLOW_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::RED, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, led_conf.max_int_intensity_ptc);
        led_conf.int_led_pin_out.reset(LedPinColor::BLUE);
        break;

    case LedColor::WHITE_COLOR:
        led_conf.int_led_pin_out.set(LedPinColor::ALL, led_conf.max_int_intensity_ptc);
        break;
    default:
        return;
    }
}

void LedPeriphery::set_duty_cycle_pct(float duty_cycle_fraction){
    led_conf.duty_cycle_ptc = duty_cycle_fraction * 100;
    led_conf.duty_cycle = led_conf.blink_period * duty_cycle_fraction;
}

void LedPeriphery::set_blink_period(uint32_t period){
    led_conf.blink_period = period;
    set_duty_cycle_pct(led_conf.duty_cycle_ptc / 100.0);
}

void LedPeriphery::toggle_rgb_internal(uint8_t red, uint8_t green, uint8_t blue){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (state==GPIO_PIN_SET){
        led_conf.int_led_pin_out.reset(LedPinColor::ALL);
        return;
    }

    uint8_t int_red     = float(red * led_conf.max_int_intensity_ptc) / (led_conf.max_red);
    uint8_t int_green   = float(green* led_conf.max_int_intensity_ptc) / (led_conf.max_green);
    uint8_t int_blue    = float(blue * led_conf.max_int_intensity_ptc) / (led_conf.max_blue);
    
    if (int_red == 0){
        led_conf.int_led_pin_out.reset(LedPinColor::RED);

    } else {
        led_conf.int_led_pin_out.set(LedPinColor::RED, int_red);
    }
    if (int_green == 0){
        led_conf.int_led_pin_out.reset(LedPinColor::GREEN);
    } else {
        led_conf.int_led_pin_out.set(LedPinColor::GREEN, int_green);
    }
    if (int_blue == 0){
        led_conf.int_led_pin_out.reset(LedPinColor::BLUE);
    } else {
        led_conf.int_led_pin_out.set(LedPinColor::BLUE, int_blue);
    }
}


void LedPeriphery::toggle_rgb_external(uint8_t red, uint8_t green, uint8_t blue){
    auto crnt_time_ms = HAL_GetTick();
    GPIO_PinState state = (crnt_time_ms % led_conf.blink_period > led_conf.duty_cycle) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (state==GPIO_PIN_SET){
        led_conf.int_led_pin_out.reset(LedPinColor::ALL);
        return;
    }
    
    uint8_t ext_red     = float(red * led_conf.max_ext_intensity_ptc) / (led_conf.max_red);
    uint8_t ext_green   = float(green* led_conf.max_ext_intensity_ptc) / (led_conf.max_green);
    uint8_t ext_blue    = float(blue * led_conf.max_ext_intensity_ptc) / (led_conf.max_blue);
    if (ext_red == 0){
        led_conf.ext_led_pin_out.reset(LedPinColor::RED);
    } else {
        led_conf.ext_led_pin_out.set(LedPinColor::RED, ext_red);
    }

    if (ext_green == 0){
        led_conf.ext_led_pin_out.reset(LedPinColor::GREEN);
    } else {
        led_conf.ext_led_pin_out.set(LedPinColor::GREEN, ext_green);
    }

    if (ext_blue == 0){
        led_conf.ext_led_pin_out.reset(LedPinColor::BLUE);
    } else {
        led_conf.ext_led_pin_out.set(LedPinColor::BLUE, ext_blue);
    }
}

//  TODO: delete state and color from the set(), and just use toggle gpio version instead to set a color. 
//  TODO: delete versions of LedPorts classes, since they are identical.