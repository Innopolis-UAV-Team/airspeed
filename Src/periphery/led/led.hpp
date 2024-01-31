/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022-2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef SRC_APPLICATION_PERIPHERY_LED_HPP_
#define SRC_APPLICATION_PERIPHERY_LED_HPP_

#include "main.h"
#include <stdint.h>
#include "periphery/pwm/pwm.hpp"
#include "logger.hpp"

enum class LedColor {
    RED_COLOR,
    GREEN_COLOR,
    BLUE_COLOR,
    CYAN_COLOR,
    MAGENTA_COLOR,
    YELLOW_COLOR,
    WHITE_COLOR,
    COLORS_AMOUNT,
};

enum LedPinColor{
    RED,
    GREEN,
    BLUE,
    ALL
};

enum class PortType {
    PWM,
    GPIO,
    UINT_16
};

class LedInterface {
public:
    virtual void set_red(uint8_t intensity, GPIO_PinState state) = 0;
    virtual void set_blue(uint8_t intensity, GPIO_PinState state) = 0;
    virtual void set_green(uint8_t intensity, GPIO_PinState state) = 0;
    virtual void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) = 0;

    virtual void reset_red() = 0;
    virtual void reset_blue() = 0;
    virtual void reset_green() = 0;
    virtual void reset(LedPinColor color) = 0;
};

#ifdef STM32F103xB
class LedPortsINT: public LedInterface{
public:
    static PwmPin port_red;
    static GPIO_TypeDef* port_green;
    static GPIO_TypeDef* port_blue;

    static uint16_t pin_red;
    static uint16_t pin_green;
    static uint16_t pin_blue;

    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;

private:
    void set_red(uint8_t intensity, GPIO_PinState state) override;
    void set_blue(uint8_t intensity, GPIO_PinState state) override;
    void set_green(uint8_t intensity, GPIO_PinState state) override;

    void reset_red() override;
    void reset_blue() override;
    void reset_green() override;
};

#ifdef EXT_RGB_LED_RED_GPIO_Port
class LedPortsEXT : public LedInterface{
public:
    static GPIO_TypeDef* port_red;
    static GPIO_TypeDef* port_green;
    static GPIO_TypeDef* port_blue;

    static uint16_t pin_red;
    static uint16_t pin_green;
    static uint16_t pin_blue;
    
    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;
private:
    void set_red(uint8_t intensity, GPIO_PinState state){};
    void set_blue(uint8_t intensity, GPIO_PinState state){};
    void set_green(uint8_t intensity, GPIO_PinState state){};

    void reset_red(){};
    void reset_blue(){};
    void reset_green(){};
};
#else
class LedPortsEXT : public LedInterface{
public:
    static PwmPin port_red;
    static PwmPin port_green;
    static PwmPin port_blue;

    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;

private:
    void set_red(uint8_t intensity, GPIO_PinState state) {};
    void set_blue(uint8_t intensity, GPIO_PinState state) {};
    void set_green(uint8_t intensity, GPIO_PinState state) {};

    void reset_red(){};
    void reset_blue(){};
    void reset_green(){};
};
#endif
#else
// #ifdef STM32F103xB
class LedPortsINT : public LedInterface{
public:
    static uint16_t port_red;
    static uint16_t* port_green;
    static uint16_t* pin_blue;

    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;

private:
    void set_red(uint8_t intensity,  GPIO_PinState state) {};
    void set_blue(uint8_t intensity,  GPIO_PinState state) {};
    void set_green(uint8_t intensity,  GPIO_PinState state) {};
    void reset_red(){};
    void reset_blue(){};
    void reset_green(){};
};

class LedPortsEXT : public LedInterface{
public:
    static uint16_t port_red;
    static uint16_t port_green;
    static uint16_t port_blue;

    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;

private:
    void set_red(uint8_t intensity,  GPIO_PinState state) {};
    void set_blue(uint8_t intensity,  GPIO_PinState state) {};
    void set_green(uint8_t intensity,  GPIO_PinState state) {};
    void reset_red(){};
    void reset_blue(){};
    void reset_green(){};
};
#endif

class LedPeriphery {
public:
    // int8_t init(bool is_internal, uint8_t led_max_intensity, uint32_t led_blink_period, uint8_t led_duty_cycle, bool use_pwm = false);
    // void reset(LedPortsOut led_pin_out);
    // static bool int_led_is_defined;
    // static bool ext_led_is_defined;
    static LedPortsINT int_led_pin_out;
    static LedPortsEXT ext_led_pin_out;

    // internal
    // toggle_internal()
    // set_internal()

    // external
    
    Logger _logger;

    static void toggle_internal(LedColor color);
    static void toggle_external(LedColor color);

    static void set_internal(LedColor color, uint8_t intensity = max_int_intensity);
    static void set_external(LedColor color, uint8_t intensity = max_int_intensity);

    static void reset_internal(LedPinColor pin_color);
    static void reset_external(LedPinColor pin_color);

    static uint8_t max_int_intensity;
    static uint8_t max_ext_intensity;

    static uint32_t blink_period;
    static uint8_t duty_cycle;
    static uint32_t _last_publish_time;
private:
    void set_intensity(uint8_t intensity, bool to_internal);
    void reset(PortType port, uint16_t pin);
};


#endif  // SRC_APPLICATION_PERIPHERY_LED_LED_HPP_
