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
|  10 | lights.ttl              | TTL of specified by pwm.cmd_type commands [ms]. |
|  11 | pwm.cmd_ttl_ms          | TTL of LightCommands [ms]. |
|  12 | pwm.frequency           | PWM frequency [Hz]. |
|  13 | pwm.cmd_type            | 0 means RawCommand, 1 means ArrayCommand, 2 is reserved for hardpoint.Command. |
|  14 | pwm.5_ch                | Index of setpoint channel. [-1; 255]. -1 means disabled, |
|  15 | pwm.5_min               | PWM duration when setpoint is min (RawCommand is 0 or Command is -1.0) |
|  16 | pwm.5_max               | PWM duration when setpoint is max (RawCommand is 8191 or Command is 1.0) |
|  17 | pwm.5_def               | PWM duration when setpoint is negative or there is no setpoint at all. |
|  18 | pwm.5_feedback          | Indicates the operational mode of the node. 0 means disabled. When set to 1, the command of corresponding Status type for cmd_type will be transmitted (esc.RawCommand - esc.Status, actuator.ArrayCommand - actuator.Status) with frequency 1 Hz. When set to 2 - 10 Hz. |
|  19 | dprs.offset             | Offset for pressure sensor in mpa. |
|  20 | range_finder.id         | Range finder sensor ID, used in uavcan.equipment.range_sensor.Measurement publications. |
|  21 | range_finder.enable     | Indicates the operational mode of the node. When set to 1, the uavcan.equipment.range_sensor will be transmitted. |

> This docs was automatically generated. Do not edit it manually.

