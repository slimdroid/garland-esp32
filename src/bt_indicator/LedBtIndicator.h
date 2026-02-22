#pragma once

#include <Arduino.h>
#include "IBtIndicator.h"

/**
 * @brief Bluetooth indicator implementation using a regular LED (for esp32devkit)
 *
 * @tparam PIN GPIO pin number of the built-in LED (compile-time constant)
 */
template<uint8_t PIN>
class LedBtIndicator : public IBtIndicator {
public:
    LedBtIndicator()
        : _lastBlinkTime(0),
          _blinkIntervalMs(0),
          _blinkState(HIGH) {
        pinMode(PIN, OUTPUT);
    }

    void setState(BT_ConnectionState state) override {
        switch (state) {
            case BT_ENABLED:
                _blinkIntervalMs = 1000;
                break;
            case BT_CONNECTED:
                _blinkIntervalMs = 200;
                break;
            case BT_DISCONNECTED:
                _blinkIntervalMs = 1000;
                break;
            case BT_DISABLED:
                _blinkIntervalMs = 0;
                digitalWrite(PIN, LOW);
                _blinkState = LOW;
                break;
        }
    }

    void handle() override {
        if (_blinkIntervalMs <= 0) {
            return;
        }
        if (millis() - _lastBlinkTime >= static_cast<unsigned long>(_blinkIntervalMs)) {
            _lastBlinkTime = millis();
            _blinkState = !_blinkState;
            digitalWrite(PIN, _blinkState);
        }
    }

private:
    unsigned long _lastBlinkTime;
    int _blinkIntervalMs;
    bool _blinkState;
};
