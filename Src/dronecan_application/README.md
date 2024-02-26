The node has the following registers:

| №  | Register name           | Description |
| -- | ----------------------- | ----------- |
|  1 | uavcan.node.id          | Defines a node-ID. Allowed values [0,127]. |
|  2 | system.name             | Defines custom node name. If empty, the node will use the default name. |
|  3 | light.id                | Defines the light id. |
|  4 | lights.max_intensity    | Defines the max intensity of the lights [%]. |
|  5 | lights.default_color    | Defines a default color of the lights. |
|  6 | lights.type             | Defines an intensity transition rate of the lights. |
|  7 | lights.blink_period_ms  | Defines a blink period of the lights [ms]. |
|  8 | lights.duty_cycle_pct   | Defines a duty cycle of the lights [%]. |

> This docs was automatically generated. Do not edit it manually.

