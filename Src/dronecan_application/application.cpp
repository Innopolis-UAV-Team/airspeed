/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "application.hpp"
#include "dronecan.h"
#include "logger.hpp"
#include "main.h"
#include "modules/CircuitStatusModule.hpp"
#include "modules/LightsModule.hpp"
#include "modules/DifferentialPressure/DifferentialPressure.hpp"
#include "params.hpp"
#include "modules/PWMModule.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

void application_entry_point() {
    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT),
               NUM_OF_STR_PARAMS, -1, 1);
    paramsLoad();

    auto node_id =
        paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "arl.lights";
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);

    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    uavcanInitApplication(node_id);

    LightsModule& light_module = LightsModule::get_instance();
    CircuitStatusModule& status_module = CircuitStatusModule::get_instance();
    DifferentialPressure& pressure_module = DifferentialPressure::get_instance();
    PWMModule& pwm_module = PWMModule::get_instance();
    while (true) {
        light_module.spin_once();
        status_module.spin_once();
        pressure_module.spin_once();
        uavcanSetNodeHealth((NodeStatusHealth_t) pressure_module.status);        pwm_module.spin_once();
        uavcanSetNodeHealth((NodeStatusHealth_t)(pwm_module.module_status));
        uavcanSpinOnce();

#ifdef HAL_IWDG_MODULE_ENABLED
        HAL_IWDG_Refresh(&hiwdg);
#endif  // HAL_IWDG_MODULE_ENABLED
    }
}
