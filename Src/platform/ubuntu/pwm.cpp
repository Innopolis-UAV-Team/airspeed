/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#include "periphery/pwm/pwm.hpp"

#include <cstddef>
uint16_t pwm[static_cast<int>(PwmPin::PWM_AMOUNT)];
uint16_t pwm_freq[static_cast<int>(PwmPin::PWM_AMOUNT)];
Logger PwmPeriphery::_logger = Logger("pwmPeriphery");
void PwmPeriphery::init(PwmPin) {}

void PwmPeriphery::set_duration(const PwmPin pwm_pin, uint16_t duration_us) {
    pwm[static_cast<int>(pwm_pin)] = duration_us;
}

uint16_t PwmPeriphery::get_duration(PwmPin pwm_pin) {
    return pwm[static_cast<int>(pwm_pin)];
}
void PwmPeriphery::set_duty_cycle_pct(PwmPin pwm_pin, uint8_t duty_cycle_pct) {
    if (duty_cycle_pct > 100) {
        return;
    }

    float fraction = 1.0 - (duty_cycle_pct / 100.0);

    pwm[static_cast<int>(pwm_pin)] = (fraction * 1000);
}

void PwmPeriphery::reset(PwmPin pwm_pin) {
    pwm[static_cast<int>(pwm_pin)] = 1000;
}

uint16_t PwmPeriphery::get_frequency(PwmPin pwm_pin) {
    return pwm_freq[static_cast<int>(pwm_pin)];
}

void PwmPeriphery::set_frequency(PwmPin pwm_pin, uint16_t frequency) {
    pwm_freq[static_cast<int>(pwm_pin)] = 1000000 / frequency;
}
