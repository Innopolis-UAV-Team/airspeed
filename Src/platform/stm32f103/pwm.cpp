/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/pwm/pwm.hpp"
#include "main.h"

// #ifdef htim2
extern TIM_HandleTypeDef htim2;
// #endif
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

// PWM_1,      // PA8 TIM1_CH1 INT_LED_GREEN
// PWM_2,      // PA15 TIM2_CH1 INT_LED_RED
// PWM_3,      // PB0 TIM3_CH3, TIM1_CH2N
// PWM_4,      // PB4 TIM3_CH1
// PWM_5,      // PB5 TIM3_CH2
    
Logger PwmPeriphery::_logger = Logger("PWM");

void PwmPeriphery::set_duration(const PwmPin pwm_pin, uint32_t duration_us) {
    switch (pwm_pin) {
        case PwmPin::PWM_1:
            TIM1->CCR1 = duration_us;
            break;

        case PwmPin::PWM_2:
            #ifdef htim2
            TIM2->CCR1 = duration_us;
            #endif
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

uint32_t PwmPeriphery::get_duration(PwmPin pwm_pin) {
    uint32_t pwm_duration;

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            pwm_duration = TIM1->CCR2;
            break;

        case PwmPin::PWM_2:
            #ifdef htim2
            pwm_duration = TIM2->CCR1;
            #endif
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

    float fraction = duty_cycle_pct/100;

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            // _HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int) fraction * TIM1->ARR - 1);
            TIM1->CCR2 = (int) fraction * TIM1->ARR - 1;
            break;

        case PwmPin::PWM_2:
            // _HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (int) fraction * TIM4->ARR - 1);
            // #ifdef htim2
            TIM2->CCR1 = (int) fraction * TIM2->ARR - 1;
            // #endif
            break;

        case PwmPin::PWM_3:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR3 = (int) fraction * TIM3->ARR - 1;

            break;

        case PwmPin::PWM_4:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR1 = (int) fraction * TIM3->ARR - 1;
            break;

        case PwmPin::PWM_5:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (int) fraction * TIM3->ARR - 1);
            TIM3->CCR2 = (int) fraction * TIM3->ARR - 1;
            break;
        default:
            _logger.log_debug("No such PIN");
            break;
    }
}

void PwmPeriphery::reset(PwmPin pwm_pin){

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            // TIM1->CCR1 = 1;

            // HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
            __HAL_TIM_DISABLE_IT(&htim1, TIM_CHANNEL_1);
            // HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
            break;
        
        case PwmPin::PWM_2:
            // TIM2->CCR1 = 1;

            // #ifdef htim2
            __HAL_TIM_DISABLE_IT(&htim1, TIM_CHANNEL_1);

            // HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
            // HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_1);

            // #endif
            break;

        case PwmPin::PWM_3:
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
            HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);



            break;

        case PwmPin::PWM_4:
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);

            break;

        case PwmPin::PWM_5:
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
            
            break;
        default:
            _logger.log_debug("No such PIN");
            break;
    }
}