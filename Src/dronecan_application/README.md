The node has the following registers:

| №  | Register name           | Description |
| -- | ----------------------- | ----------- |
|  1 | uavcan.node.id          | Defines a node-ID. Allowed values [0,127]. |
|  2 | system.name             | Defines custom node name. |
|  3 | lights.id               | Defines the lights id. The default value is zero, which for PX4 0 means UI LED. |
|  4 | lights.max_intensity    | Defines the max intensity of the lights [%]. |
|  5 | lights.default_color    | Defines a default color of the lights. 0 - red, 1 - green, 2 - blue, 3 - cyan, 4 - magenta, 5 - yellow, 6 - white, 7 - turned off |
|  6 | lights.type             | Defines an intensity transition rate of the lights. 0 - solid (steady and continuous illumination), 1 - blinking (rapid on-off cycling of colors), 2 - pulsing (gradual increase and decrease in brightness or color intensity). |
|  7 | lights.blink_period_ms  | Defines a blink period of the lights [ms]. This parameter has an effect only when lights.type=1 (blinking) |
|  8 | lights.duty_cycle_pct   | Defines a duty cycle of the lights [%]. |

> This docs was automatically generated. Do not edit it manually.

