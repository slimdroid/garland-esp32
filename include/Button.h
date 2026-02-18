#pragma once

enum ButtonAction {
    NO_ACTION,
    SHORT_PRESS,
    MEDIUM_PRESS,
    LONG_PRESS
};

class Button {
public:
    explicit Button(uint8_t pin, uint8_t mode = INPUT_PULLDOWN);

    ButtonAction handle();

private:
    uint8_t _pin;
    bool _lastState;
    bool _isPressed;
    bool _longPressSent;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;

    static constexpr unsigned long DEBOUNCE_MS = 50;
    static constexpr unsigned long MEDIUM_PRESS_MS = 1000;
    static constexpr unsigned long LONG_PRESS_MS = 5000;
};