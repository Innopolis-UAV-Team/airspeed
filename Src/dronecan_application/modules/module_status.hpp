/***
 * Copyright (C) 2024 Anastasiia Stepanova  <asiiapine@gmail.com>
 * Distributed under the terms of the GPL v3 license, available in the file LICENSE.
***/

#ifndef SRC_DRONECAN_APPLICATION_MODULES_MODULE_STATUS_HPP_
#define SRC_DRONECAN_APPLICATION_MODULES_MODULE_STATUS_HPP_

enum class ModuleStatus: uint8_t {
    MODULE_OK        = 0,   // The module is functioning properly
    MODULE_WARN      = 1,   // The module encountered a minor failure
    MODULE_ERROR     = 2,   // The module encountered a major failure
    MODULE_CRITICAL  = 3,   // The module suffered a fatal malfunction
};
#endif  // SRC_DRONECAN_APPLICATION_MODULES_MODULE_STATUS_HPP_
