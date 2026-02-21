#pragma once

/**
 * @brief Management of RGB LED string
 */
namespace RgbIndicator {
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
    void init(uint8_t pinR, uint8_t pinG, uint8_t pinB);

    /**
     * @brief Periodic handler to update LED colors based on current mode and state
     * @param mode Current lighting mode
     * @param isSystemOff true if the system should be dark
     */
    void handleLEDs(LightMode mode, bool isSystemOff);
}
