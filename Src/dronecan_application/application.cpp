// Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
// Distributed under the terms of the GPL v3 license, available in the file LICENSE.

#include "application.hpp"
#include "dronecan.h"
#include "main.h"
#include "params.hpp"
#include "logger.hpp"

#include "periphery/led/led.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;

void application_entry_point() {
    Logger logger = Logger("APP");
    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    auto node_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "co.raccoonlab.mini";
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);
    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    uint16_t int_intensity = 50;
    uint16_t int_max_intensity = 1000;
    extern LedData led_conf;

    led_conf.max_int_intensity = int_max_intensity;
    led_conf.max_ext_intensity = int_max_intensity;
    led_conf.duty_cycle_ptc = 50;
    LedPeriphery::set_blink_period(1000);
    led_conf._led_logger = Logger("LED");
    led_conf.int_current_color = LedColor::COLORS_AMOUNT;

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    uavcanInitApplication(node_id);
   
    HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_SET);
    

    char buffer[90];
    uint32_t last_intensity_change = HAL_GetTick();
    LedPeriphery::reset_internal();

    while(true) {
        
        // HAL_GPIO_WritePin(INT_RGB_LED_BLUE_GPIO_Port, INT_RGB_LED_BLUE_Pin, GPIO_PIN_SET);
        // TIM1->CCR1 = HAL_GetTick() % 1000;
        // TIM2->CCR1 = HAL_GetTick() % 1000;
        if ( HAL_GetTick() - last_intensity_change>500){
            sprintf(buffer, "TIM1->CCR1: %d", TIM1->CCR1);
            logger.log_info(buffer);
            sprintf(buffer, "TIM2->CCR1: %d", TIM2->CCR1);
            logger.log_info(buffer);
            last_intensity_change = HAL_GetTick();
            // led_conf.int_led_pin_out.reset(LedPinColor::ALL);
        }

        // int_intensity = HAL_GetTick() % 1000;
        // LedPeriphery::set_internal(int_intensity);
        // led_conf.int_led_pin_out.set(LedPinColor::ALL, int_intensity);
        LedPeriphery::toggle_internal(LedColor::RED_COLOR);
        uavcanSpinOnce();

#ifdef HAL_IWDG_MODULE_ENABLED
        HAL_IWDG_Refresh(&hiwdg);
#endif  // HAL_IWDG_MODULE_ENABLED
    }
}
