/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file
 *  LICENSE.
 ***/

#include "RangeFinder.hpp"

RangeFinder RangeFinder::instance = RangeFinder();
bool RangeFinder::instance_initialized = false;
Logger RangeFinder::logger = Logger("RangeFinder");
ModuleStatus RangeFinder::status = ModuleStatus::MODULE_OK;

RangeFinder& RangeFinder::get_instance() {
    if (!instance_initialized) {
        instance.init();
    }
    return instance;
}

void RangeFinder::init() {
    msg = {.timestamp = 0,
           .sensor_type = RangeFinderSensorType::SENSOR_TYPE_LIDAR,
           .reading_type = 1};
    if (garminLiteInit() < 0) {
        status = ModuleStatus::MODULE_WARN;
        return;
    }
    instance_initialized = true;
    update_params();
}

void RangeFinder::spin_once() {
    update_params();

    if (!enabled) {
        return;
    }
    update_data();
    if (status != ModuleStatus::MODULE_OK) {
        return;
    }
    publish();
}

void RangeFinder::update_data() {
    static uint32_t next_upd_ms = 100;
    if (HAL_GetTick() < next_upd_ms) {
        return;
    }
    status = ModuleStatus::MODULE_OK;
    next_upd_ms = HAL_GetTick() + 100;
    if (garminLiteCollectData() < 0) {
        status = ModuleStatus::MODULE_WARN;
        init();
        return;
    }
    range_m = garminLiteParseCollectedData();
}

void RangeFinder::update_params() {
    static uint32_t next_upd_ms = 100;
    if (HAL_GetTick() < next_upd_ms) {
        return;
    }
    msg.sensor_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_RANGE_FINDER_ID);
    enabled = paramsGetIntegerValue(IntParamsIndexes::PARAM_RANGE_FINDER_ENABLE);
    next_upd_ms = HAL_GetTick() + 1000;
}

void RangeFinder::publish() {
    static uint32_t next_pub_ms = 100;
    static uint8_t transfer_id = 0;
    if (HAL_GetTick() < next_pub_ms) {
        return;
    }
    msg.range = range_m;
    publish_error = dronecan_equipment_range_sensor_measurement_publish(&msg, &transfer_id);
    transfer_id++;

    next_pub_ms = HAL_GetTick() + 100;
}
