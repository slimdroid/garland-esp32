#pragma once

#include <FastLED.h>
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
 * @tparam PIN GPIO pin number of the RGB LED (compile-time constant required by FastLED)
 */
template<uint8_t PIN>
class RgbLedBtIndicator : public IBtIndicator {
public:
    RgbLedBtIndicator()
        : _blinkIntervalMs(0),
          _lastBlinkTime(0),
          _blinkState(false) {
        FastLED.addLeds<WS2812, PIN, GRB>(_leds, 1).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(50);
        _leds[0] = CRGB::Black;
        FastLED.show();
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
                _leds[0] = CRGB::Black;
                FastLED.show();
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
            _leds[0] = _blinkState ? CRGB::Blue : CRGB::Black;
            FastLED.show();
        }
    }

private:
    CRGB _leds[1];
    int _blinkIntervalMs;
    unsigned long _lastBlinkTime;
    bool _blinkState;
};
