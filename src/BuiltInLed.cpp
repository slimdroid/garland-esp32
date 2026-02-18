#include <Arduino.h>
#include "BuiltInLed.h"

BuiltInLed::BuiltInLed(uint8_t pin)
    : _pin(pin),
      _lastBlinkTime(0),
      _blinkIntervalMs(0),
      _blinkState(HIGH) {
    pinMode(_pin, OUTPUT);
}

void BuiltInLed::switchOn(bool blink, int intervalMs) {
    if (blink) {
        _blinkIntervalMs = intervalMs;
    } else {
        _blinkIntervalMs = 0;
    }
    if (_blinkState != HIGH) {
        digitalWrite(_pin, HIGH);
        _blinkState = HIGH;
    }
}

void BuiltInLed::switchOff() {
    _blinkIntervalMs = 0;
    digitalWrite(_pin, LOW);
    _blinkState = LOW;
}

void BuiltInLed::handle() {
    if (_blinkIntervalMs <= 0) {
        digitalWrite(_pin, LOW);
        _blinkState = LOW;
        return;
    }
    if (millis() - _lastBlinkTime >= static_cast<unsigned long>(_blinkIntervalMs)) {
        _lastBlinkTime = millis();
        _blinkState = !_blinkState;
        digitalWrite(_pin, _blinkState);
    }
}
