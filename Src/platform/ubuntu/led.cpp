/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"

uint16_t LedPortsINT::port_red = 0;
uint16_t LedPortsINT::port_green = 1;
uint16_t LedPortsINT::port_blue = 2;


uint16_t LedPortsEXT::port_red = 3;
uint16_t LedPortsEXT::port_green = 4;
uint16_t LedPortsEXT::port_blue = 5;

LedData led_conf;

void LedPeriphery::toggle_internal(LedColor color){}
void LedPeriphery::toggle_external(LedColor color){}

void LedPeriphery::set_internal(LedColor color, uint8_t intensity){}
void LedPeriphery::set_external(LedColor color, uint8_t intensity){}

void LedPeriphery::reset_internal(LedPinColor pin_color){}
void LedPeriphery::reset_external(LedPinColor pin_color){}
