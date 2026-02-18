#pragma once

#define LOG(tag, format, ...) Serial.printf("[%s] " format "\n", tag, ##__VA_ARGS__)