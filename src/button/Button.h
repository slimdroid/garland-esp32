#pragma once

/**
 * @brief Button action types
 */
enum ButtonAction {
    NO_ACTION,    ///< No action detected
    SHORT_PRESS,  ///< Short press detected
    MEDIUM_PRESS, ///< Medium press detected
    LONG_PRESS    ///< Long press detected
};

/**
 * @brief Class for handling button presses with debouncing and multi-stage press detection
 */
class Button {
public:
    /**
     * @param pin GPIO pin for the button
     */
    explicit Button(uint8_t pin);

    /**
     * @brief Periodic handler to process button state and detect actions
     * @return The detected ButtonAction
     */
    ButtonAction handle();

private:
    uint8_t _pin;
    bool _activeLow;
    bool _lastState;
    bool _isPressed;
    bool _longPressSent;
    unsigned long _lastDebounceTime;
    unsigned long _pressStartTime;

    static constexpr unsigned long DEBOUNCE_MS = 50;
    static constexpr unsigned long MEDIUM_PRESS_MS = 1000;
    static constexpr unsigned long LONG_PRESS_MS = 5000;
};