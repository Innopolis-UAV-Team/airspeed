/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/pwm/pwm.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

// PWM_1,      // PA8 TIM1_CH1 INT_LED_GREEN
// PWM_2,      // PA15 TIM2_CH1
// PWM_3,      // PB0 TIM3_CH3, TIM1_CH2N
// PWM_4,      // PB4 TIM3_CH1
// PWM_5,      // PB5 TIM3_CH2
    
Logger PwmPeriphery::_logger;
// int8_t PwmPeriphery::init(PwmPin pwm_pin) {
//     _logger.init("PWM");
//     switch (pwm_pin) {
//         case PwmPin::PWM_1:
//             if (HAL_OK != HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)) {
//                 return -1;
//             }
//             TIM1->CCR2 = 1000;
//             break;

//         case PwmPin::PWM_2:
//             if (HAL_OK != HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1)) {
//                 return -1;
//             }
//             TIM2->CCR1 = 1000;
//             break;

//         case PwmPin::PWM_3:
//             if (HAL_OK != HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3)) {
//                 return -1;
//             }
//             TIM3->CCR3 = 1000;
//             break;

//         case PwmPin::PWM_4:
//             if (HAL_OK != HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1)) {
//                 return -1;
//             }
//             TIM3->CCR1 = 1000;
//             break;
//         case PwmPin::PWM_5:
//             if (HAL_OK != HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2)) {
//                 return -1;
//             }
//             TIM3->CCR2 = 1000;
//             break;
//         default:
//             return -1;
//     }

//     return 0;
// }

void PwmPeriphery::set_duration(const PwmPin pwm_pin, uint32_t duration_us) {
    switch (pwm_pin) {
        case PwmPin::PWM_1:
            TIM1->CCR2 = duration_us;
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
}

uint32_t PwmPeriphery::get_duration(PwmPin pwm_pin) {
    uint32_t pwm_duration;

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

    float fraction = duty_cycle_pct/100;

    switch (pwm_pin) {
        case PwmPin::PWM_1:
            // _HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int) fraction * TIM1->ARR - 1);
            TIM1->CCR2 = (int) fraction * TIM1->ARR - 1;
            break;

        case PwmPin::PWM_2:
            // _HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (int) fraction * TIM4->ARR - 1);
            TIM2->CCR1 = (int) fraction * TIM2->ARR - 1;
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
            // _HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (int) fraction * TIM1->ARR - 1);
            HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1);
            // __HAL_TIM_DISABLE_IT(htim1, TIM_IT_CC3);

            break;

        case PwmPin::PWM_2:
            HAL_TIM_PWM_Stop_IT(&htim2, TIM_CHANNEL_1);
            
            // _HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (int) fraction * TIM4->ARR - 1);
            // TIM4->CCR1 = (int) fraction * TIM4->ARR - 1;
            break;

        case PwmPin::PWM_3:
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);

            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (int) fraction * TIM3->ARR - 1);
            // TIM3->CCR3 = (int) fraction * TIM3->ARR - 1;

            break;

        case PwmPin::PWM_4:
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);

            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (int) fraction * TIM3->ARR - 1);
            // TIM3->CCR1 = (int) fraction * TIM3->ARR - 1;
            break;

        case PwmPin::PWM_5:
            // _HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, (int) fraction * TIM3->ARR - 1);
            HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_2);
            
            // TIM3->CCR2 = (int) fraction * TIM3->ARR - 1;
            break;
        default:
            _logger.log_debug("No such PIN");
            break;
    }
}