/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "PressureModule.hpp"

PressureModule PressureModule::instance = PressureModule();
bool PressureModule::instance_initialized = false;
Logger PressureModule::logger = Logger("PressureModule");

PressureModule& PressureModule::get_instance() {
    if (!instance_initialized) {
        instance_initialized = true;
        instance.init();
    }
    return instance;
}

void PressureModule::init() {
    int8_t pressure_sensor_status = pressure_driver.init();
    if (pressure_sensor_status != 0) {
        logger.log_error("DPS init");
        status = ModuleStatus::MODULE_CRITICAL;
    }
}

void PressureModule::spin_once() {
    publish_data();
    update_params();
}

void PressureModule::update_params() {
    static uint32_t next_upd_ms = 1000;
    if (HAL_GetTick() < next_upd_ms) {
        return;
    }

    offset = paramsGetIntegerValue(IntParamsIndexes::PARAM_DPRS_OFFSET);
    next_upd_ms = HAL_GetTick() + 1000;
}

void PressureModule::publish_data() {
    static uint32_t next_pub_ms = 100;

    if (HAL_GetTick() < next_pub_ms) {
        return;
    }

    if (pressure_driver.get_data() != 0) {
        status = ModuleStatus::MODULE_WARN;
        logger.log_error("get_data");
        return;
    }

    air_data.differential_pressure = pressure_driver.data.pressure - offset;
    air_data.differential_pressure_sensor_temperature = pressure_driver.data.temperature;
    if (dronecan_equipment_air_data_raw_air_data_publish(&air_data, &transfer_id) == 0) {
        transfer_id++;
    } else {
        logger.log_error("data_not_sent");
        status = ModuleStatus::MODULE_WARN;
    }
    next_pub_ms = HAL_GetTick() + 100;

}