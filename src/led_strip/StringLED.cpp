#include <Arduino.h>
#include "StringLED.h"

// use 13 bit precision for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency (8191 max)
#define LEDC_BASE_FREQ     1000

namespace StringLED {
    // For non-blocking fade
    static int fadeValue = 0;
    static int fadeDirection = 1;
    static unsigned long lastFadeUpdate = 0;
    static constexpr int fadeInterval = 10;
    static uint8_t _pinR, _pinG, _pinB;

    // Arduino like analogWrite
    // value has to be between 0 and valueMax
    static void ledcAnalogWrite(uint8_t pin, uint32_t value, uint32_t valueMax = 255) {
        // calculate duty based on 13-bit resolution (8191 max)
        uint32_t duty = ((1 << LEDC_TIMER_13_BIT) - 1) * min(value, valueMax) / valueMax;
        // write duty to LEDC
        ledcWrite(pin, duty);
    }

    void init(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
        _pinR = pinR;
        _pinG = pinG;
        _pinB = pinB;

        // Setup timer and attach timer to a led pins
        ledcAttach(_pinR, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
        ledcAttach(_pinG, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
        ledcAttach(_pinB, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
    }

    void handleLEDs(LightMode mode, bool isSystemOff) {
        if (isSystemOff) {
            ledcAnalogWrite(_pinR, 0);
            ledcAnalogWrite(_pinG, 0);
            ledcAnalogWrite(_pinB, 0);
        } else {
            switch (mode) {
                case FADE:
                    if (millis() - lastFadeUpdate >= fadeInterval) {
                        lastFadeUpdate = millis();
                        fadeValue += fadeDirection;
                        if (fadeValue <= 0 || fadeValue >= 255) {
                            fadeDirection *= -1;
                        }
                        ledcAnalogWrite(_pinR, fadeValue);
                        ledcAnalogWrite(_pinG, fadeValue);
                        ledcAnalogWrite(_pinB, fadeValue);
                    }
                    break;

                case RED:
                    ledcAnalogWrite(_pinR, 255);
                    ledcAnalogWrite(_pinG, 0);
                    ledcAnalogWrite(_pinB, 0);
                    break;

                case GREEN:
                    ledcAnalogWrite(_pinR, 0);
                    ledcAnalogWrite(_pinG, 255);
                    ledcAnalogWrite(_pinB, 0);
                    break;

                case BLUE:
                    ledcAnalogWrite(_pinR, 0);
                    ledcAnalogWrite(_pinG, 0);
                    ledcAnalogWrite(_pinB, 255);
                    break;

                case WHITE:
                    ledcAnalogWrite(_pinR, 255);
                    ledcAnalogWrite(_pinG, 255);
                    ledcAnalogWrite(_pinB, 255);
                    break;

                default:
                    break;
            }
        }
    }
}
