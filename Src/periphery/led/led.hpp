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
class LedPortsINT : public LedInterface{
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

class LedPortsEXT : public LedInterface{
public:
    static uint16_t port_red;
    static uint16_t port_green;
    static uint16_t port_blue;

    void set(LedPinColor color, uint8_t intensity, GPIO_PinState state) override;
    void reset(LedPinColor color) override;

private:
    void set_red(uint8_t intensity,  GPIO_PinState state){};
    void set_blue(uint8_t intensity,  GPIO_PinState state){};
    void set_green(uint8_t intensity,  GPIO_PinState state){};
    void reset_red(){};
    void reset_blue(){};
    void reset_green(){};
};
#endif

struct LedData
{
    Logger _led_logger;

    LedPortsINT int_led_pin_out=LedPortsINT();
    LedPortsEXT ext_led_pin_out = LedPortsEXT();

    uint8_t max_int_intensity;
    uint8_t max_ext_intensity;

    uint32_t blink_period = 1000;
    uint8_t duty_cycle = 50;
};


namespace LedPeriphery{

    // internal
    // toggle_internal()
    // set_internal()

    // external

    void toggle_internal(LedColor color);
    void toggle_external(LedColor color);

    void set_internal(LedColor color, uint8_t intensity);
    void set_external(LedColor color, uint8_t intensity);

    void reset_internal(LedPinColor pin_color);
    void reset_external(LedPinColor pin_color);
    void set_intensity(uint8_t intensity, bool to_internal);
};


#endif  // SRC_APPLICATION_PERIPHERY_LED_LED_HPP_
