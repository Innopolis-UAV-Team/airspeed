#ifndef SRC_APPLICATION_PERIPHERY_PWM_HPP_
#define SRC_APPLICATION_PERIPHERY_PWM_HPP_

#include <stdint.h>
#include "logger.hpp"

#ifdef __cplusplus
extern "C" {
#endif

enum class GPIOPin{
    INT_RGB_LED_RED,    //
    INT_RGB_LED_GREEN,  //
    INT_RGB_LED_BLUE,   //
};

class GPIOPeriphery{
    static Logger _logger;
public:
    // GPIOPeriphery();
    // GPIO_TypeDef* port;
    // uint16_t pin;
    static void set(const GPIOPin gpio_pin);
    static GPIO_PinState get(const GPIOPin gpio_pin);
    static void toggle(GPIOPin gpio_pin, uint32_t blink_period, uint32_t duty_cycle);
    static void reset(GPIOPin gpio_pin);
};

#ifdef __cplusplus
}
#endif

#endif  // SRC_APPLICATION_PERIPHERY_PWM_HPP_