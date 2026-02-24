#pragma once

#include <Adafruit_NeoPixel.h>
#include <board/m5nanoc6/Pins.h>
#include "IBtIndicator.h"

/**
 * @brief Bluetooth indicator implementation using an RGB LED (for m5nanoc6 and m5atoms3)
 *
 * States:
 *   BT_ENABLED      — blue slow blink (1000ms)
 *   BT_CONNECTED    — blue fast blink (200ms)
 *   BT_DISCONNECTED — blue slow blink (1000ms)
 *   BT_DISABLED     — LED off
 *
 * @tparam PIN GPIO pin number of the RGB LED
 */
template<uint8_t PIN>
class RgbLedBtIndicator : public IBtIndicator {
public:
    RgbLedBtIndicator()
        : _pixel(1, PIN, NEO_GRB + NEO_KHZ800),
          _blinkIntervalMs(0),
          _lastBlinkTime(0),
          _blinkState(false) {
#ifdef BOARD_M5_NANOC6
        pinMode(Pins::LED_PWR, OUTPUT);
        digitalWrite(Pins::LED_PWR, HIGH);
#endif

        _pixel.begin();
        _pixel.setPixelColor(0, 0);
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
                _blinkState = false;
                _pixel.setPixelColor(0, 0);
                _pixel.show();
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
            _pixel.setPixelColor(0, _blinkState ? _pixel.Color(0, 0, 255) : 0);
            _pixel.show();
        }
    }

private:
    Adafruit_NeoPixel _pixel;
    int _blinkIntervalMs;
    unsigned long _lastBlinkTime;
    bool _blinkState;
};
