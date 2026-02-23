#include <Arduino.h>
#include "Button.h"

Button::Button(const uint8_t pin)
    : _pin(pin),
      _activeLow(true),
      _lastState(HIGH),
      _isPressed(false),
      _longPressSent(false),
      _shortPressSent(false),
      _lastDebounceTime(0),
      _pressStartTime(0),
      _isSystemOff(nullptr) {
    pinMode(pin, INPUT_PULLUP);
}

void Button::setSystemState(const bool *isSystemOff) {
    _isSystemOff = isSystemOff;
}

ButtonAction Button::handle() {
    ButtonAction action = NO_ACTION;
    bool currentState = digitalRead(_pin);
    if (_activeLow) {
        currentState = !currentState;
    }
    const unsigned long now = millis();

    if (currentState != _lastState) {
        _lastDebounceTime = now;
    }

    if ((now - _lastDebounceTime) > DEBOUNCE_MS) {
        if (currentState != _isPressed) {
            _isPressed = currentState;

            if (_isPressed) {
                _pressStartTime = now;
                _longPressSent = false;
                _shortPressSent = false;
            } else {
                if (!_shortPressSent) {
                    const unsigned long duration = now - _pressStartTime;
                    if (duration < MEDIUM_PRESS_MS) {
                        action = SHORT_PRESS;
                    } else if (duration < LONG_PRESS_MS) {
                        action = MEDIUM_PRESS;
                    }
                }
            }
        }

        if (_isPressed && !_shortPressSent && _isSystemOff && !*_isSystemOff) {
            if ((now - _pressStartTime) >= MEDIUM_PRESS_MS) {
                _shortPressSent = true;
                action = MEDIUM_PRESS;
            }
        }

        if (_isPressed && !_longPressSent) {
            if ((now - _pressStartTime) >= LONG_PRESS_MS) {
                _longPressSent = true;
                action = LONG_PRESS;
            }
        }
    }

    _lastState = currentState;
    return action;
}
