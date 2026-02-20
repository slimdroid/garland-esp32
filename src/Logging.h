#pragma once

/**
 * @brief Simple logging macro
 * @param tag Component tag for log message
 * @param format Format string (printf-style)
 * @param ... Arguments for the format string
 */
#define LOG(tag, format, ...) Serial.printf("[%s] " format "\n", tag, ##__VA_ARGS__)