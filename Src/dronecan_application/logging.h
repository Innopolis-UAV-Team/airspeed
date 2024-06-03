#ifndef SRC_LOGGING_H_
#define SRC_LOGGING_H_

#include <stdarg.h>

#include "uavcan/protocol/debug/LogMessage.h"

static inline bool set_source(DebugLogMessage_t msg, const char *str) {
    if (str == nullptr) {
        return false;
    }

    size_t str_len = strlen(str);
    if (strlen(str) > 31) {
        return false;
    }

    snprintf((char *)msg.source, str_len, str);
    msg.source_size = str_len;
    return true;
}

static inline bool set_text(DebugLogMessage_t msg, const char *str) {
    if (str == nullptr) {
        return false;
    }
    size_t str_len = strlen(str);

    if (str_len > 90) {
        return false;
    }

    snprintf((char *)msg.text, str_len, str);
    msg.text_size = str_len;
    return true;
}
#endif  // SRC_LOGGING_H_
