/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/led/led.hpp"

void LedPeriphery::toggle_internal(LedColor color){}
void LedPeriphery::toggle_external(LedColor color){}

void LedPeriphery::set_internal(LedColor color, uint8_t intensity){}
void LedPeriphery::set_external(LedColor color, uint8_t intensity){}

void LedPeriphery::reset_internal(LedPinColor pin_color){}
void LedPeriphery::reset_external(LedPinColor pin_color){}
