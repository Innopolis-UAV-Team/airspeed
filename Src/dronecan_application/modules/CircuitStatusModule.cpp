/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine96@gmail.com>
 *  Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/ 

#include "CircuitStatusModule.hpp"


CircuitStatusModule CircuitStatusModule::instance = CircuitStatusModule();
bool CircuitStatusModule::instance_initialized = false;
Logger CircuitStatusModule::logger = Logger("CircuitStatus");
LightsModule* CircuitStatusModule::light_module = &LightsModule::getInstance();

CircuitStatusModule& CircuitStatusModule::getInstance() {
    if(!instance_initialized){
        light_module = &LightsModule::getInstance();
        instance_initialized=true;
        instance.init();
    }
    return instance;
}

void CircuitStatusModule::init(){
    int8_t adc_status = adc.init();
    if (adc_status != 0){
        logger.log_error("ADC init");
    } else {
        temp_raw = adc.get(AdcChannel::ADC_TEMPERATURE);
        temp = stm32TemperatureParse(temp_raw);
        vol_raw = adc.get(AdcChannel::ADC_VIN);
        cur_raw = adc.get(AdcChannel::ADC_CURRENT);
        circuit_status = {.voltage = AdcPeriphery::stm32Voltage(vol_raw), .current=AdcPeriphery::stm32Current(cur_raw)};
        dronecan_equipment_circuit_status_publish(&circuit_status, &circuit_status_pub_id);
        circuit_status_pub_id +=1;
    }
}

void CircuitStatusModule::spin_once(){
    RgbSimpleColor color = RgbSimpleColor::BLUE_COLOR;

    if (v5_f > 5.5 || circuit_status.voltage > 21.5) {
        circuit_status.error_flags = ERROR_FLAG_OVERVOLTAGE;
    } else if (circuit_status.current > 1.05) {
        light_module->ext_led_driver.reset();
        circuit_status.error_flags = ERROR_FLAG_OVERCURRENT;
    } else if (circuit_status.current < 0.01) {
        circuit_status.error_flags = ERROR_FLAG_UNDERCURRENT;
    } else if (publish_error) {
        logger.log_debug("pub");
    }

    if (circuit_status.error_flags != ERROR_FLAG_CLEAR){
        color = RgbSimpleColor::YELLOW_COLOR;
    }

    if (HAL_GetTick()%1000 == 0) {
        temp_raw = adc.get(AdcChannel::ADC_TEMPERATURE);
        temp = AdcPeriphery::stm32Temperature(temp_raw);
        temperature_status.temperature = temp;
        
        publish_error = dronecan_equipment_temperature_publish(&temperature_status, &temperature_pub_id);
        if (publish_error) {
            color = RgbSimpleColor::RED_COLOR;
        } else {
            temperature_pub_id ++;
        } 
    }
    
    if (HAL_GetTick()%1000 == 0) {
        v5 = adc.get(AdcChannel::ADC_5V);
        v5_f =  AdcPeriphery::stm32Voltage5V(v5);

        vol_raw = adc.get(AdcChannel::ADC_VIN);
        cur_raw = adc.get(AdcChannel::ADC_CURRENT);

        circuit_status.voltage = AdcPeriphery::stm32Voltage(vol_raw);
        circuit_status.current=AdcPeriphery::stm32Current(cur_raw);

        publish_error = dronecan_equipment_circuit_status_publish(&circuit_status, &circuit_status_pub_id);
        if (publish_error) {
            color = RgbSimpleColor::RED_COLOR;
        } else {
            circuit_status_pub_id ++;
        } 
    }
    
    circuit_status.error_flags = ERROR_FLAG_CLEAR;
    light_module->int_led_driver.set(color);
}

