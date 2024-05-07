/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "DifferentialPressure.hpp"

DifferentialPressure DifferentialPressure::instance = DifferentialPressure();
bool DifferentialPressure::instance_initialized = false;
Logger DifferentialPressure::logger = Logger("DifferentialPressure");

DifferentialPressure& DifferentialPressure::get_instance() {
    if (!instance_initialized) {
        instance_initialized = true;
        instance.init();
    }
    return instance;
}

void DifferentialPressure::init() {
    int8_t pressure_sensor_status = ms4525doInit();
    if (pressure_sensor_status != 0) {
        logger.log_debug("DPS init");
        status = ModuleStatus::MODULE_CRITICAL;
    }
}

void DifferentialPressure::spin_once() {
    publish_data();
    update_params();
}

void DifferentialPressure::update_params() {
    static uint32_t next_upd_ms = 1000;
    if (HAL_GetTick() < next_upd_ms) {
        return;
    }

    offset = paramsGetIntegerValue(IntParamsIndexes::PARAM_DPRS_OFFSET)/1000.0f;
    next_upd_ms = HAL_GetTick() + 1000;
}

void DifferentialPressure::publish_data() {
    static uint32_t next_pub_ms = 100;

    if (HAL_GetTick() < next_pub_ms) {
        return;
    }

    if (ms4525CollectData() != 0) {
        status = ModuleStatus::MODULE_WARN;
        logger.log_debug("get_data");
        return;
    }

    dprs_data = ms4525ParseCollectedData();
    air_data.differential_pressure = dprs_data.diff_pressure + offset;
    air_data.differential_pressure_sensor_temperature = dprs_data.temperature;
    if (dronecan_equipment_air_data_raw_air_data_publish(&air_data, &transfer_id) == 0) {
        transfer_id++;
    } else {
        logger.log_debug("data_not_sent");
        status = ModuleStatus::MODULE_WARN;
    }
    next_pub_ms = HAL_GetTick() + 100;
}
