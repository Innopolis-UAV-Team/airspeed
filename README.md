# VTOL Lights DroneCAN application

> Developers docs: [DroneCAN template application](https://github.com/RaccoonlabDev/mini_v2_node/wiki/dronecan).

### 1. Key features

- You can run the software in SITL mode (don't need to have any hardware),

### 2. Brief hardware info

| View | Drw |
| ---- | ------ |
| <img src="assets/view.png" alt="drawing" height="200"> | ... |

### 3. Application interface

The node subscribes on [LightsCommand](https://dronecan.github.io/Specification/7._List_of_standard_data_types/#lightscommand) and blinks the external LEDs with PWM.

The node has the following parameters:
- lights.max_intensity
- lights.default_color
- lights.type
- lights.blink_period_ms
- lights.duty_cycle_pct
