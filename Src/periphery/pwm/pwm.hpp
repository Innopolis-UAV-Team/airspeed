/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef SRC_APPLICATION_PERIPHERY_PWM_HPP_
#define SRC_APPLICATION_PERIPHERY_PWM_HPP_

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
    PWM_2,      // PA15 TIM2_CH1
    PWM_3,      // PB0 TIM3_CH3, TIM1_CH2N
    PWM_4,      // PB4 TIM3_CH1
    PWM_5,      // PB5 TIM3_CH2
    PWM_AMOUNT,
};


class PwmPeriphery {
    static Logger _logger;
public:
    // static int8_t init(PwmPin pin);
    static void set_duration(const PwmPin pwm_pin, uint16_t duration_us);
    static uint16_t get_duration(PwmPin pwm_pin);
    static void set_duty_cycle(PwmPin pwm_pin, uint8_t duty_cycle_pct);
    static void reset(PwmPin pwm_pin);
};

#ifdef __cplusplus
}
#endif

#endif  // SRC_APPLICATION_PERIPHERY_PWM_HPP_
