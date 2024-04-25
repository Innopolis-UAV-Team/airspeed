/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef SRC_PERIPHERY_PWM_PWM_HPP_
#define SRC_PERIPHERY_PWM_PWM_HPP_

#include <stdint.h>
#include "logger.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @note PWM pinout related to RaccoonLab Mini v2 node
 */
enum class PwmPin {
    PWM_1,      // PA8 TIM1_CH1 INT_LED_GREEN
    PWM_2,      // PA15 TIM2_CH1 INT_LED_RED
    PWM_3,      // PB5 TIM3_CH2 EXT_GREEN
    PWM_4,      // PB4 TIM3_CH1 EXT_RED
    PWM_5,      // PB3 TIM2_CH2 PWM_5
    PWM_6,      // PB0 TIM3_CH3 EXT_BLUE
    PWM_AMOUNT = 7,
};


class PwmPeriphery {
    static Logger _logger;
public:
    static void init(PwmPin pin);
    static void set_duration(const PwmPin pwm_pin, uint16_t duration_us);
    static void set_duty_cycle_pct(PwmPin pwm_pin, uint8_t duty_cycle_pct);
    static void reset(PwmPin pwm_pin);
    static uint16_t get_duration(PwmPin pwm_pin);
    static uint16_t get_frequency(PwmPin pwm_pin);
    static void set_frequency(PwmPin pwm_pin, uint16_t frequency);
};

#ifdef __cplusplus
}
#endif

#endif  // SRC_PERIPHERY_PWM_PWM_HPP_
