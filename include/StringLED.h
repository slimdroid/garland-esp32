#pragma once

// LED PINs
#define LED_PIN_R   21
#define LED_PIN_G   22
#define LED_PIN_B   23

namespace StringLED {
    // Modes
    enum LightMode {
        FADE,
        RED,
        GREEN,
        BLUE,
        WHITE,
        NUM_MODES
    };

    void init(
        uint8_t pinR = LED_PIN_R,
        uint8_t pinG = LED_PIN_G,
        uint8_t pinB = LED_PIN_B
    );

    void handleLEDs(LightMode mode, bool isSystemOff);
}
