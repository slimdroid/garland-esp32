#pragma once

class BuiltInLed {
public:
    explicit BuiltInLed(uint8_t pin = LED_BUILTIN);

    void switchOn(bool blink = false, int intervalMs = 0);

    void switchOff();

    void handle();

private:
    uint8_t _pin;
    unsigned long _lastBlinkTime;
    int _blinkIntervalMs;
    bool _blinkState;
};
