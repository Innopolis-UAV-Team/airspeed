/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "application.hpp"
#include "dronecan.h"
#include "main.h"
#include "params.hpp"
#include "logger.hpp"
#include "modules/LightsModule.hpp"
#include "modules/CircuitStatusModule.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

void application_entry_point() {

    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    auto node_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "arl.lights";
    Logger logger = Logger(node_name);
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);

    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    uavcanInitApplication(node_id);
    
<<<<<<< HEAD
    // LightsModule& light_module = LightsModule::get_instance();

    // CircuitStatusModule& status_module = CircuitStatusModule::get_instance();
    char buffer[90];
    while(true) {
        // light_module.spin_once();
        if (HAL_GetTick() % 1000 ==0 ){
            logger.log_debug("Hi");

        }
        // status_module.spin_once();

=======
    LightsModule& light_module = LightsModule::get_instance();
    CircuitStatusModule& status_module = CircuitStatusModule::get_instance();

    char buffer[90];
    while(true) {
        light_module.spin_once();
        status_module.spin_once();
>>>>>>> 37a65a2 (apply suggestions)
        uavcanSpinOnce();

        #ifdef HAL_IWDG_MODULE_ENABLED
        HAL_IWDG_Refresh(&hiwdg);
        #endif  // HAL_IWDG_MODULE_ENABLED
    }
}