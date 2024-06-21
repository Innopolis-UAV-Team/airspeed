/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapineil.com>
 * Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/

#ifndef SRC_DRONECAN_APPLICATION_MODULES_PWMMODULE_HPP_
#define SRC_DRONECAN_APPLICATION_MODULES_PWMMODULE_HPP_

#include <algorithm>

#include "logger.hpp"
#include "dronecan.h"
#include "params.hpp"
#include "../module_status.hpp"
#include "periphery/pwm/pwm.hpp"
#include "periphery/servos/common.hpp"
#include "uavcan/equipment/actuator/ArrayCommand.h"
#include "uavcan/equipment/esc/RawCommand.h"
#include "uavcan/equipment/esc/Status.h"
#include "uavcan/equipment/indication/LightsCommand.h"
#include "uavcan/equipment/actuator/Status.h"

struct PwmChannelInfo {
    PwmPin pin;
    uint16_t min;
    uint16_t max;
    uint16_t def;
    int16_t channel;
    uint16_t command_val;
    uint32_t cmd_end_time_ms;
    uint32_t next_status_pub_ms;
    uint8_t fb;
};

struct PwmChannelsParamsNames {
    IntegerParamValue_t min;
    IntegerParamValue_t max;
    IntegerParamValue_t def;
    IntegerParamValue_t ch;
    IntegerParamValue_t fb;
};

class PWMModule {
public:
    void spin_once();
    static PWMModule& get_instance();
    static PwmChannelInfo params;
    static PwmChannelsParamsNames params_names;
    static ModuleStatus module_status;

protected:
    PWMModule();

private:
    static PWMModule instance;
    void (*callback)(CanardRxTransfer*);
    void (*publish_state)();

    void init();
    void update_params();
    void update_pwm();
    void apply_params();

    static void raw_command_callback(CanardRxTransfer* transfer);
    static void array_command_callback(CanardRxTransfer* transfer);

    static void publish_esc_status();
    static void publish_actuator_status();
    static void publish_raw_command();
    static void publish_array_command();

    static uint16_t pwm_freq;
    static uint8_t pwm_cmd_type;

    static uint16_t ttl_cmd;
    uint16_t status_pub_timeout_ms;
    bool verbose;

    static bool publish_error;
    static Logger logger;

    PWMModule& operator=(const PWMModule&) = delete;
    explicit PWMModule(PWMModule* other) = delete;
};
#endif  // SRC_DRONECAN_APPLICATION_MODULES_PWMMODULE_HPP_
