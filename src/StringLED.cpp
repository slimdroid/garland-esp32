#include <Arduino.h>
#include "StringLED.h"

// use first 3 channels of 16 channels (started from zero)
#define LEDC_CHANNEL_0_R  0
#define LEDC_CHANNEL_1_G  1
#define LEDC_CHANNEL_2_B  2

// use 13 bit precision for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency (8191 max)
#define LEDC_BASE_FREQ     5000

namespace StringLED {
    // For non-blocking fade
    static int fadeValue = 0;
    static int fadeDirection = 1;
    static unsigned long lastFadeUpdate = 0;
    static constexpr int fadeInterval = 10;

    // Arduino like analogWrite
    // value has to be between 0 and valueMax
    static void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
        // calculate duty based on 13-bit resolution (8191 max)
        uint32_t duty = (LEDC_BASE_FREQ * min(value, valueMax)) / valueMax;
        // write duty to LEDC
        ledcWrite(channel, duty);
    }

    void init(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
        // Setup timer and attach timer to a led pins
        ledcSetup(LEDC_CHANNEL_0_R, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
        ledcAttachPin(pinR, LEDC_CHANNEL_0_R);
        ledcSetup(LEDC_CHANNEL_1_G, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
        ledcAttachPin(pinG, LEDC_CHANNEL_1_G);
        ledcSetup(LEDC_CHANNEL_2_B, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
        ledcAttachPin(pinB, LEDC_CHANNEL_2_B);
    }

    void handleLEDs(LightMode mode, bool isSystemOff) {
        if (isSystemOff) {
            ledcAnalogWrite(LEDC_CHANNEL_0_R, 0);
            ledcAnalogWrite(LEDC_CHANNEL_1_G, 0);
            ledcAnalogWrite(LEDC_CHANNEL_2_B, 0);
        } else {
            switch (mode) {
                case FADE:
                    if (millis() - lastFadeUpdate >= fadeInterval) {
                        lastFadeUpdate = millis();
                        fadeValue += fadeDirection;
                        if (fadeValue <= 0 || fadeValue >= 255) {
                            fadeDirection *= -1;
                        }
                        ledcAnalogWrite(LEDC_CHANNEL_0_R, fadeValue);
                        ledcAnalogWrite(LEDC_CHANNEL_1_G, fadeValue);
                        ledcAnalogWrite(LEDC_CHANNEL_2_B, fadeValue);
                    }
                    break;

                case RED:
                    ledcAnalogWrite(LEDC_CHANNEL_0_R, 255);
                    ledcAnalogWrite(LEDC_CHANNEL_1_G, 0);
                    ledcAnalogWrite(LEDC_CHANNEL_2_B, 0);
                    break;

                case GREEN:
                    ledcAnalogWrite(LEDC_CHANNEL_0_R, 0);
                    ledcAnalogWrite(LEDC_CHANNEL_1_G, 255);
                    ledcAnalogWrite(LEDC_CHANNEL_2_B, 0);
                    break;

                case BLUE:
                    ledcAnalogWrite(LEDC_CHANNEL_0_R, 0);
                    ledcAnalogWrite(LEDC_CHANNEL_1_G, 0);
                    ledcAnalogWrite(LEDC_CHANNEL_2_B, 255);
                    break;

                case WHITE:
                    ledcAnalogWrite(LEDC_CHANNEL_0_R, 255);
                    ledcAnalogWrite(LEDC_CHANNEL_1_G, 255);
                    ledcAnalogWrite(LEDC_CHANNEL_2_B, 255);
                    break;

                default:
                    break;
            }
        }
    }
}
