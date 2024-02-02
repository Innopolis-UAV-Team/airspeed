/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/pwm/pwm.hpp"
#include "main.h"


// PWM_1,      // PA8 TIM1_CH1 INT_LED_GREEN
// PWM_2,      // PA15 TIM2_CH1 INT_LED_RED
// PWM_3,      // PB0 TIM3_CH3, TIM1_CH2N
// PWM_4,      // PB4 TIM3_CH1
// PWM_5,      // PB5 TIM3_CH2
    
Logger PwmPeriphery::_logger = Logger("PWM");

void PwmPeriphery::set_duration(const PwmPin pwm_pin, uint16_t duration_us) {
    switch (pwm_pin) {
        case PwmPin::PWM_1:
            TIM1->CCR1 = duration_us;
            break;

        case PwmPin::PWM_2:
            TIM2->CCR1 = duration_us;
            break;

        case PwmPin::PWM_3:
            TIM3->CCR3 = duration_us;
            break;

        case PwmPin::PWM_4:
            TIM3->CCR1 = duration_us;
            break;        

        case PwmPin::PWM_5:
            TIM3->CCR2 = duration_us;
            break;
        default:
            break;
    }
    // char buffer[90];
    // sprintf(buffer, "%d", duration_us);
    // _logger.log_info(buffer);
}

uint16_t PwmPeriphery::get_duration(PwmPin pwm_pin) {
    uint16_t pwm_duration;

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            pwm_duration = TIM1->CCR2;
            break;

        case PwmPin::PWM_2:
            pwm_duration = TIM2->CCR1;
            break;

        case PwmPin::PWM_3:
            pwm_duration = TIM3->CCR3;
            break;

        case PwmPin::PWM_4:
            pwm_duration = TIM3->CCR1;
            break;        
        
        case PwmPin::PWM_5:
            pwm_duration = TIM3->CCR2;
            break;

        default:
            pwm_duration = 0;
            break;
    }

    return pwm_duration;
}

void PwmPeriphery::set_duty_cycle(PwmPin pwm_pin, uint8_t duty_cycle_pct){
    
    if (duty_cycle_pct > 100){
        return;
    }

    float fraction = 1.0 - (duty_cycle_pct / 100.0);

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            // _HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int) fraction * TIM1->ARR - 1);
            TIM1->CCR1 = (fraction * TIM1->ARR);
            break;

        case PwmPin::PWM_2:
            // _HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (int) fraction * TIM4->ARR - 1);
            TIM2->CCR1 = (fraction * TIM1->ARR);
            break;

        case PwmPin::PWM_3:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR3 = (fraction * TIM1->ARR);
            break;

        case PwmPin::PWM_4:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR1 = (fraction * TIM1->ARR);
            break;

        case PwmPin::PWM_5:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR2 = (fraction * TIM1->ARR);
            break;
        default:
            _logger.log_debug("No such PIN");
            break;
    }
}

void PwmPeriphery::reset(PwmPin pwm_pin){

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            TIM1->CCR1 = TIM1->ARR;
            break;
        
        case PwmPin::PWM_2:
            TIM2->CCR1 = TIM2->ARR;
            break;

        case PwmPin::PWM_3:
            TIM3->CCR3 = 0;
            break;

        case PwmPin::PWM_4:
            TIM3->CCR1 = 0;
            break;

        case PwmPin::PWM_5:
            TIM3->CCR2 = 0;
            break;

        default:
            _logger.log_debug("No such PIN");
            break;
    }
}