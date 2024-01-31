// Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
// Distributed under the terms of the GPL v3 license, available in the file LICENSE.

#include "application.hpp"
#include "dronecan.h"
#include "main.h"
#include "params.hpp"
#include "periphery/led/led.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

void application_entry_point() {
    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    auto node_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "co.raccoonlab.mini";
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);
    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    uint8_t int_intensity = 50;
    uint8_t int_max_intensity = 100;
    LedPeriphery::max_int_intensity = int_max_intensity;
    LedPeriphery::max_ext_intensity = int_max_intensity;
    LedPeriphery::blink_period = 1000;
    LedPeriphery::duty_cycle = 50;
    uavcanInitApplication(node_id);
    uint32_t last_intensity_change = HAL_GetTick();

    while(true) {
        
        if ( HAL_GetTick() - last_intensity_change>1000){
            if (int_intensity >= 100){
                int_intensity = 20;
            }
            LedPeriphery::set_internal(LedColor::BLUE_COLOR, int_intensity);
            int_intensity+=20;
            last_intensity_change = HAL_GetTick();
        }
        
        // led.toggle(LedColor::GREEN_COLOR);
        uavcanSpinOnce();

#ifdef HAL_IWDG_MODULE_ENABLED
        HAL_IWDG_Refresh(&hiwdg);
#endif  // HAL_IWDG_MODULE_ENABLED
    }
}
