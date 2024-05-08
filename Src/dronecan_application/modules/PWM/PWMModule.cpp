/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/

#include "PWMModule.hpp"

#define CHANNEL(channel) IntParamsIndexes::PARAM_PWM_##channel##_CH
#define MIN(channel) IntParamsIndexes::PARAM_PWM_##channel##_MIN
#define MAX(channel) IntParamsIndexes::PARAM_PWM_##channel##_MAX
#define DEF(channel) IntParamsIndexes::PARAM_PWM_##channel##_DEF
#define FB(channel) IntParamsIndexes::PARAM_PWM_##channel##_FB

Logger PWMModule::logger = Logger("PWMModule");

uint16_t PWMModule::ttl_cmd = 0;
uint16_t PWMModule::pwm_freq = 1000;
uint8_t PWMModule::pwm_cmd_type = 0;

ModuleStatus PWMModule::module_status = ModuleStatus::MODULE_OK;

PWMModule PWMModule::instance = PWMModule();

PWMModule::PWMModule() {
    update_params();
    init();
}

PwmChannelInfo PWMModule::params = {.pin = PwmPin::PWM_5};

PwmChannelsParamsNames PWMModule::params_names = {
    .min = MIN(5), .max = MAX(5), .def = DEF(5), .ch = CHANNEL(5), .fb = FB(5)};

PWMModule& PWMModule::get_instance() {
    static bool instance_initialized = false;
    if (!instance_initialized) {
        instance_initialized = true;
        instance.init();
    }
    return instance;
}

void PWMModule::init() {
    logger.init("PWMModule");
    PwmPeriphery::init(params.pin);
    uavcanSubscribe(UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_SIGNATURE, UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_ID,
                                                                            raw_command_callback);
    uavcanSubscribe(UAVCAN_EQUIPMENT_ACTUATOR_ARRAY_COMMAND_SIGNATURE,
                                                        UAVCAN_EQUIPMENT_ACTUATOR_ARRAY_COMMAND_ID,
                                                        array_command_callback);
}

void PWMModule::spin_once() {
    uint32_t crnt_time_ms = HAL_GetTick();

    static uint32_t next_update_ms = 0;
    if (crnt_time_ms > next_update_ms) {
        next_update_ms = crnt_time_ms + 1000;
        instance.update_params();
        instance.apply_params();
    }

    if (crnt_time_ms > params.cmd_end_time_ms) {
        params.command_val = params.def;
    }
    PwmPeriphery::set_duration(params.pin, params.command_val);

    status_pub_timeout_ms = 1;
    static uint32_t next_pub_ms = 5000;

    if (module_status == ModuleStatus::MODULE_OK &&
        crnt_time_ms > next_pub_ms) {
        publish_state();
        next_pub_ms = crnt_time_ms + status_pub_timeout_ms;
    }
}

void PWMModule::update_params() {
    module_status = ModuleStatus::MODULE_OK;
    bool params_error = false;

    pwm_freq = paramsGetIntegerValue(IntParamsIndexes::PARAM_PWM_FREQUENCY);
    pwm_cmd_type = paramsGetIntegerValue(IntParamsIndexes::PARAM_PWM_CMD_TYPE);

    ttl_cmd = paramsGetIntegerValue(IntParamsIndexes::PARAM_PWM_CMD_TTL_MS);
    params.fb = paramsGetIntegerValue(params_names.fb);
    params.min = paramsGetIntegerValue(params_names.min);
    params.max = paramsGetIntegerValue(params_names.max);
    params.def = paramsGetIntegerValue(params_names.def);
    auto channel = paramsGetIntegerValue(params_names.ch);

    status_pub_timeout_ms = 100;
    uint8_t max_channel = 255;

    switch (pwm_cmd_type) {
        case 0:
            max_channel = NUMBER_OF_RAW_CMD_CHANNELS - 1;
            break;
        default:
            break;
    }

    static uint32_t last_warn_pub_time_ms = 0;

    if (channel < max_channel) {
        params.channel = channel;
    } else {
        params.channel = max_channel;
        params_error = true;
    }
    if (params_error) {
        module_status = ModuleStatus::MODULE_WARN;
        if (last_warn_pub_time_ms < HAL_GetTick()) {
            last_warn_pub_time_ms = HAL_GetTick() + 10000;
            logger.log_debug("check parameters");
        }
    }
}

void PWMModule::apply_params() {
    if (PwmPeriphery::get_frequency(params.pin) != pwm_freq) {
        PwmPeriphery::set_frequency(params.pin, pwm_freq);
    }
    switch (pwm_cmd_type) {
        case 0:
            publish_state = publish_esc_status;
            break;

        case 1:
            publish_state = publish_actuator_status;
            break;

        default:
            return;
    }
}

void PWMModule::publish_esc_status() {
    static uint8_t transfer_id = 0;
    EscStatus_t msg{};
    auto crnt_time_ms = HAL_GetTick();
    static uint32_t
        next_status_pub_ms;
    if (params.channel < 0 || params.fb == 0 || next_status_pub_ms > crnt_time_ms) {
        return;
    }
    msg.esc_index = params.channel;
    auto pwm_val = PwmPeriphery::get_duration(params.pin);
    auto scaled_value = mapPwmToPct(pwm_val, params.min, params.max);
    msg.power_rating_pct = (uint8_t)(scaled_value);
    if (dronecan_equipment_esc_status_publish(&msg, &transfer_id) == 0) {
        transfer_id++;
        next_status_pub_ms = crnt_time_ms + ((params.fb > 1) ? 100 : 1000);
    }
}

void PWMModule::publish_actuator_status() {
    static uint8_t transfer_id = 0;
    ActuatorStatus_t msg {};
    auto crnt_time_ms = HAL_GetTick();
    static uint32_t
        next_status_pub_ms;
    if (params.channel < 0 || params.fb == 0 || next_status_pub_ms > crnt_time_ms) {
        return;
    }
    msg.actuator_id = params.channel;
    auto pwm_val = PwmPeriphery::get_duration(params.pin);
    auto scaled_value = mapPwmToPct(pwm_val, params.min, params.max);
    msg.power_rating_pct = (uint8_t) scaled_value;
    if (dronecan_equipment_actuator_status_publish(&msg, &transfer_id) == 0) {
        transfer_id++;
    }
}

void PWMModule::raw_command_callback(CanardRxTransfer* transfer) {
    if (module_status != ModuleStatus::MODULE_OK || pwm_cmd_type != 0) {
        return;
    }

    int16_t cmd;
    if (!dronecan_equipment_esc_raw_command_channel_deserialize(transfer, params.channel, &cmd)) {
        return;
    }

    params.cmd_end_time_ms = HAL_GetTick() + ttl_cmd;
    params.command_val = mapRawCommandToPwm(cmd, params.min, params.max, params.def);
}

void PWMModule::array_command_callback(CanardRxTransfer* transfer) {
    if (module_status != ModuleStatus::MODULE_OK || pwm_cmd_type != 1) return;
    ArrayCommand_t command;
    auto pwm = &params;
    int8_t ch_num = dronecan_equipment_actuator_arraycommand_deserialize(
        transfer, &command);
    if (ch_num <= 0 || pwm->channel < 0) {
        return;
    }
    for (uint8_t j = 0; j < ch_num; j++) {
        if (command.commads[j].actuator_id != pwm->channel) {
            continue;
        }
        pwm->cmd_end_time_ms = HAL_GetTick() + ttl_cmd;
        pwm->command_val = mapActuatorCommandToPwm(
            command.commads[j].command_value, pwm->min, pwm->max, pwm->def);
    }
}
