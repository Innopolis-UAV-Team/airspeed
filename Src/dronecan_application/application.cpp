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

#ifdef HAL_IWDG_MODULE_ENABLED
extern IWDG_HandleTypeDef hiwdg;
#endif /* HAL_IWDG_MODULE_ENABLED */

#ifdef STM32F103xB
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
// # TODO: delete
extern ADC_HandleTypeDef hadc1;

#endif
Logger logger = Logger("APP");
int8_t res_prev = 0;
bool error = false;
LightsCommand_t command = {};
volatile uint16_t ADC_DMA[2] = { 0 };
static inline uint8_t adc_dma_buffer[static_cast<uint8_t>(AdcChannel::ADC_NUMBER_OF_CNANNELS)];

void callback(CanardRxTransfer* transfer) {
    LightsCommand_t raw_command;
    char buffer[90];
    int8_t res = dronecan_equipment_indication_lights_command_deserialize(transfer, &raw_command);
    if (res > 0) {
        res_prev = res;
        command = raw_command;
        // Do something very quickly, or save the command for later use
    } else {
        error = true;
        // Handle a real time error
    }
}

void change_color(LedColor &color){
    int a = HAL_GetTick() % 6000;
    if (a == 2000){
        color = LedColor::BLUE_COLOR;
    } else if (a==4000) {
        color = LedColor::GREEN_COLOR;

    } else if (a==0) {
        color = LedColor::RED_COLOR;
    }
}

void application_entry_point() {
    char buffer[90];

    paramsInit(static_cast<uint8_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT), NUM_OF_STR_PARAMS);
    paramsLoadFromFlash();

    auto node_id = paramsGetIntegerValue(IntParamsIndexes::PARAM_UAVCAN_NODE_ID);

    const auto node_name = "co.raccoonlab.mini";
    auto node_name_param_idx = static_cast<ParamIndex_t>(IntParamsIndexes::INTEGER_PARAMS_AMOUNT);
    paramsSetStringValue(node_name_param_idx, 19, (const uint8_t*)node_name);
    uavcanSetNodeName(node_name);

    extern LedData led_conf;
    auto default_color = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DEFAULT_COLOR);
    auto blink_period = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_BLINK_PERIOD_MS);
    led_conf.max_ext_intensity_ptc = paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_MAX_INTENSITY);
    led_conf.duty_cycle_ptc =  paramsGetIntegerValue(IntParamsIndexes::PARAM_LIGHTS_DUTY_CYCLE_PTC);
    
    LedPeriphery::set_blink_period(blink_period);

    led_conf.max_int_intensity_ptc = 100;
    led_conf._led_logger = Logger("LED");

    uavcanInitApplication(node_id);
   

    #ifdef STM32F103xB
    HAL_StatusTypeDef status = HAL_OK;
    status = HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

    if (status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1)");
    }
    status =  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    if (status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1)");
    }
    status = HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    if (status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1)");
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    if (status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3)");
    }
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    if (status != HAL_OK){
        logger.log_error("HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2)");
    }

    #endif

    LedPeriphery::reset_internal();
    LedPeriphery::reset_external();
    LedColor color = LedColor(default_color);

    // auto sub_id = uavcanSubscribe(UAVCAN_EQUIPMENT_INDICATION_LIGHTS_COMMAND, callback);
    // if (sub_id < 0) {
    //     logger.log_error("sub_id < 0");
    //     // Handle an initialization error
    // }

    auto last_send = HAL_GetTick();

    LedPeriphery::toggle_external(color);
    LedPeriphery::toggle_internal(color);

    AdcPeriphery adc;
    int8_t in_status = adc.init();
    if (in_status != 0){
        logger.log_error("adc.init");
    } else {
        uint16_t temp = adc.get(AdcChannel::ADC_TEMPERATURE);
    }

    while(true) {

        if (HAL_GetTick()%1000==0){
            if (in_status != 0){
            logger.log_error(buffer);
            } else {
                uint16_t temp = adc.get(AdcChannel::ADC_TEMPERATURE);
                sprintf(buffer, "temp: %d", temp);
                logger.log_info(buffer);
            }
            
        }
        LedPeriphery::toggle_external(color);
        LedPeriphery::toggle_internal(color);
        
        uavcanSpinOnce();
    }

    #ifdef HAL_IWDG_MODULE_ENABLED
    HAL_IWDG_Refresh(&hiwdg);
    #endif  // HAL_IWDG_MODULE_ENABLED


}