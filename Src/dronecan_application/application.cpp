/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "application.hpp"

#include "dronecan.h"
#include "logger.hpp"
#include "main.h"
#include "modules/CircuitStatus/CircuitStatus.hpp"
#include "modules/Lights/Lights.hpp"
#include "modules/DifferentialPressure/DifferentialPressure.hpp"
#include "modules/RangeFinder/RangeFinder.hpp"
#include "modules/PWM/PWMModule.hpp"
#include "params.hpp"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

void application_entry_point() {
    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT),
               NUM_OF_STR_PARAMS, -1, 1);
    // paramsInitRedundantPage(126);
    // paramsChooseRom();
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
    RangeFinder& range_finder = RangeFinder::get_instance();

    while (true) {
        light_module.spin_once();
        status_module.spin_once();
        pressure_module.spin_once();
        pwm_module.spin_once();
        range_finder.spin_once();
        if (pressure_module.status != ModuleStatus::MODULE_OK) {
            uavcanSetNodeHealth((NodeStatusHealth_t)(pressure_module.status));
        } else {
            if (pwm_module.module_status != ModuleStatus::MODULE_OK) {
                uavcanSetNodeHealth((NodeStatusHealth_t) pwm_module.module_status);
            } else {
                if (range_finder.status != ModuleStatus::MODULE_OK) {
                    uavcanSetNodeHealth((NodeStatusHealth_t) range_finder.status);
                } else {
                    uavcanSetNodeHealth(NodeStatusHealth_t::NODE_STATUS_HEALTH_OK);
                }
            }
        }
        uavcanSpinOnce();

#ifdef HAL_IWDG_MODULE_ENABLED
        HAL_IWDG_Refresh(&hiwdg);
#endif  // HAL_IWDG_MODULE_ENABLED
    }
}
