// Copyright (C) 2023 Dmitry Ponomarev <ponomarevda96@gmail.com>
// Distributed under the terms of the GPL v3 license, available in the file LICENSE.

#include "application.hpp"
#include "dronecan.h"
#include "main.h"
#include "params.hpp"
#include "logger.hpp"

#include "periphery/led/led.hpp"
#include "periphery/adc/adc.hpp"
#include "uavcan/equipment/esc/RawCommand.h"
#include "uavcan/equipment/indication/LightsCommand.h"
#include "uavcan/equipment/power/CircuitStatus.h"

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef STM32F103xB
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
#endif

extern LedData led_conf;


int8_t res_prev = 0;
bool publish_error = false;
LightsCommand_t command = {};
Logger logger = Logger("APP");


void callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    char buffer[90];
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        res_prev = res;
        command = raw_command;
    } else {
        publish_error = true;
    }
}

LedColor change_color(LedColor color){
    int a = HAL_GetTick() % 15000;
    if (a == 5000){
        color = LedColor::BLUE_COLOR;
        led_conf._led_logger.log_debug("Color: BLUE");
    } else if (a==10000) {
        color = LedColor::GREEN_COLOR;
        led_conf._led_logger.log_debug("Color: GREEN");

    } else if (a==0) {
        color = LedColor::RED_COLOR;
        led_conf._led_logger.log_debug("Color: RED");
    }
    return color;
}

void application_entry_point() {
    char buffer[90];

    led_conf._led_logger = Logger("LED");
    CircuitStatus_t circuit_status = {};

    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    auto node_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "co.raccoonlab.mini";
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);

    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);
    auto blink_period = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);

    uint8_t max_ext_intensity_ptc = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    led_conf.duty_cycle_ptc =  paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
    
    LedPeriphery::update_ext_intensity(max_ext_intensity_ptc);
    LedPeriphery::set_blink_period(blink_period);

    led_conf.max_int_intensity_ptc = 100;

    uavcanInitApplication(node_id);
   

    #ifdef STM32F103xB
    HAL_StatusTypeDef hal_status = HAL_OK;
    hal_status = HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    if (hal_status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1)");
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    if (hal_status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3)");
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    if (hal_status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2)");
    }

    #else
    int hal_status = 0;
    #endif


    auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    if (sub_id < 0) {
        logger.log_error("sub_id < 0");
    }

    LedColor color = LedColor(default_color);
    LedPeriphery::reset_internal();
    LedPeriphery::reset_external();

    LedPeriphery::toggle_external(color);
    LedPeriphery::toggle_internal(color);

    uint8_t circuit_status_pub_id=0;

    AdcPeriphery adc;
    uint16_t vol_raw = 0;
    uint16_t temp_raw = 0;
    uint16_t cur_raw = 0;
    uint16_t temp = 0;

    int8_t adc_status = adc.init();

    if (adc_status != 0){
        logger.log_error("ADC");
        color = LedColor::RED_COLOR;
    } else {
        temp_raw = adc.get(AdcChannel::ADC_TEMPERATURE);
        temp = stm32TemperatureParse(temp_raw);
        vol_raw = adc.get(AdcChannel::ADC_VIN);
        cur_raw = adc.get(AdcChannel::ADC_CURRENT);
        circuit_status = {.voltage = AdcPeriphery::stm32Voltage(vol_raw), .current=AdcPeriphery::stm32Current(cur_raw)};
        dronecan_equipment_circuit_status_publish(&circuit_status, &circuit_status_pub_id);
        circuit_status_pub_id +=1;
    }

    LedPeriphery::toggle_internal(color);
    
    while(true) {

        if (HAL_GetTick()%1000 == 0){
            if (adc_status != 0 | hal_status != 0){
                logger.log_error("INIT");
                color = LedColor::RED_COLOR;
            } else {
                color = LedColor::BLUE_COLOR;
                temp_raw = adc.get(AdcChannel::ADC_TEMPERATURE);
                temp = AdcPeriphery::stm32Temperature(temp_raw);
                sprintf(buffer, "temp: %d K", temp);
                logger.log_info(buffer);
            }
        }
        
        if (HAL_GetTick()%100 == 0) {
            vol_raw = adc.get(AdcChannel::ADC_VIN);
            cur_raw = adc.get(AdcChannel::ADC_CURRENT);
            circuit_status = {.voltage = AdcPeriphery::stm32Voltage(vol_raw), .current=AdcPeriphery::stm32Current(cur_raw)};
            dronecan_equipment_circuit_status_publish(&circuit_status, &circuit_status_pub_id);
            if (publish_error) {
                color = LedColor::YELLOW_COLOR;
            } else {
                circuit_status_pub_id +=1;
            }
        }
        if (circuit_status.current > 1.5){
            LedPeriphery::reset_external();
            color = LedColor::YELLOW_COLOR;
        } else {
            LedPeriphery::toggle_rgb_external(command.commands[0].color.red, command.commands[0].color.green, command.commands[0].color.blue);
        }

        LedPeriphery::toggle_internal(color);
        
        uavcanSpinOnce();
    }

    #ifdef HAL_IWDG_MODULE_ENABLED
    HAL_IWDG_Refresh(&hiwdg);
    #endif  // HAL_IWDG_MODULE_ENABLED
}