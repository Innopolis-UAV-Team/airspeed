The node has the following registers:

| №  | Register name           | Description |
| -- | ----------------------- | ----------- |
|  1 | uavcan.node.id          | Defines a node-ID. Allowed values [0,127]. |
|  2 | system.name             | Defines custom node name. |
|  3 | lights.id               | Defines the lights id. The default value is zero, which for PX4 0 means UI LED. |
|  4 | lights.max_intensity    | Defines the max intensity of the lights [%]. |
|  5 | lights.default_color    | Defines a default color of the lights. 0 - red, 1 - green, 2 - blue, 3 - cyan, 4 - magenta, 5 - yellow, 6 - white, 7 - turned off |
|  6 | lights.type             | Defines an intensity transition rate of the lights. 0 - solid (steady and continuous illumination), 1 - blinking (rapid on-off cycling of colors), 2 - pulsing (gradual increase and decrease in brightness or color intensity). |
|  7 | lights.period           | Defines a blink period of the lights [ms]. This parameter has no effect when lights.type=0 (solid) |
|  8 | lights.duty_cycle_pct   | Defines a duty cycle of the lights [%]. |
|  9 | lights.verbose          | Indicates the operational mode of the node. When set to 1, the uavcan.equipment.indication.LightsCommand will be transmitted, including the current LED color and id. |
|  10 | lights.ttl              | TTL for light commands [ms]. |
|  11 | dprs.offset             | Offset for pressure sensor in mpa. |

> This docs was automatically generated. Do not edit it manually.

