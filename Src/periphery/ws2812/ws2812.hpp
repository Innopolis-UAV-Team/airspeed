#ifndef SRC_PERIPHERY_WS2812_HPP_
#define SRC_PERIPHERY_WS2812_HPP_

#include "periphery/pwm/pwm.hpp"


class WS2812Periphery{
    static Logger _logger;
public:
    WS2812Periphery(PwmPin ws2812_pwm_pin, uint8_t num_of_pins);

    static void set();
    static void get();
    static void toggle();
};

#endif  // SRC_PERIPHERY_WS2812_HPP_