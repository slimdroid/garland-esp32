#include <Arduino.h>
#include "Button.h"

Button::Button(const uint8_t pin, const uint8_t mode)
    : _pin(pin),
      _lastState(LOW),
      _isPressed(false),
      _longPressSent(false),
      _lastDebounceTime(0),
      _pressStartTime(0) {
    pinMode(pin, mode);
}

ButtonAction Button::handle() {
    ButtonAction action = NO_ACTION;
    const bool currentState = digitalRead(_pin);
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
            } else {
                const unsigned long duration = now - _pressStartTime;
                if (duration < MEDIUM_PRESS_MS) {
                    action = SHORT_PRESS;
                } else if (duration < LONG_PRESS_MS) {
                    action = MEDIUM_PRESS;
                }
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
