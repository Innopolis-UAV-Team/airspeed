/*
 * Copyright (C) 2023 Anastasiia Stepanova <asiiapine@gmail.com>
 * Distributed under the terms of the GPL v3 license, available in the file LICENSE.
 */

#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_

#include "uavcan/protocol/debug/LogMessage.h"

static inline bool set_source(DebugLogMessage_t msg, const char *str) {
    if (str == nullptr){
        return false;
    }

    size_t str_len = strlen(str);
    if (strlen(str) > 31) {
        return false;
    }

    memcpy(msg.source, str, str_len);
    msg.source_size = str_len;
    return true;
}

static inline bool set_text(DebugLogMessage_t msg, const char *str) {
    if (str == nullptr){
        return false;
    }
    size_t str_len = strlen(str);

    if (str_len > 90) {
    return false;
    }

    memcpy(msg.text, str, str_len);
    msg.text_size = str_len;
    return true;
}
#endif  // SRC_LOGGING_H_
