/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#ifndef SRC_MODULE_MODULE_FINDER_HPP_
#define SRC_MODULE_MODULE_FINDER_HPP_

#include "uavcan/equipment/range_sensor/Measurement.h"
#include "drivers/garmin_lite.h"
#include "../module_status.hpp"
#include "params.hpp"
#include "logger.hpp"


class RangeFinder {
public:
    static RangeFinder &get_instance();
    void spin_once();
    static ModuleStatus status;

private:
    static RangeFinder instance;
    static bool instance_initialized;

    RangeSensorMeasurement_t msg;

    float range_m;
    float offset;
    float scale;
    bool enabled;

    CoarseOrientation_t orientation;
    float field_of_view;
    RangeFinder(){}

    void init();
    void update_data();
    void publish();
    void update_params();
    int8_t publish_error = 0;
    static Logger logger;

    RangeFinder(const RangeFinder &other) = delete;
    void operator=(const RangeFinder &) = delete;
};

#endif  // SRC_MODULE_MODULE_FINDER_HPP_
