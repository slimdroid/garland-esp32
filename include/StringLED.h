#pragma once

// LED PINs
#define LED_PIN_R   21
#define LED_PIN_G   22
#define LED_PIN_B   23

/**
 * @brief Management of RGB LED string
 */
namespace StringLED {
    /**
     * @brief Available lighting modes
     */
    enum LightMode {
        FADE,     ///< Smoothly cycling through colors
        RED,      ///< Static red color
        GREEN,    ///< Static green color
        BLUE,     ///< Static blue color
        WHITE,    ///< Static white color
        NUM_MODES ///< Total number of modes
    };

    /**
     * @brief Initializes the LED pins and PWM channels
     * @param pinR GPIO pin for red channel
     * @param pinG GPIO pin for green channel
     * @param pinB GPIO pin for blue channel
     */
    void init(
        uint8_t pinR = LED_PIN_R,
        uint8_t pinG = LED_PIN_G,
        uint8_t pinB = LED_PIN_B
    );

    /**
     * @brief Periodic handler to update LED colors based on current mode and state
     * @param mode Current lighting mode
     * @param isSystemOff true if the system should be dark
     */
    void handleLEDs(LightMode mode, bool isSystemOff);
}
