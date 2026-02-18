#pragma once

/**
 * @brief Simple class for managing the built-in LED
 */
class BuiltInLed {
public:
    /**
     * @param pin GPIO pin where the LED is connected
     */
    explicit BuiltInLed(uint8_t pin = LED_BUILTIN);

    /**
     * @brief Switches the LED on
     * @param blink If true, the LED will blink
     * @param intervalMs Blink interval in milliseconds
     */
    void switchOn(bool blink = false, int intervalMs = 0);

    /**
     * @brief Switches the LED off
     */
    void switchOff();

    /**
     * @brief Periodic handler for blinking logic
     */
    void handle();

private:
    uint8_t _pin;
    unsigned long _lastBlinkTime;
    int _blinkIntervalMs;
    bool _blinkState;
};
