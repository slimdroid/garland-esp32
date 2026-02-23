#include "Switcher.h"
#include <Arduino.h>
#include "../board/BoardSelector.h"
#include "../effects/Effects.h"

namespace Switcher {
    static int volatile numLeds = 30;       // Number of LEDs (default)
    static int volatile brightness = 51;    // Brightness: 20% of 255 (default)
    static CRGB leds[256];                  // Maximum buffer for LEDs
    static Effects::Mode volatile g_mode = Effects::RAINBOW;
    static bool volatile g_isSystemOff = false;
    static bool volatile g_settingsChanged = false;
    static bool volatile g_numLedsChanged = false;
    static bool g_useTask = false;

    void handle_internal() {
        if (g_settingsChanged) {
            FastLED.setBrightness(brightness);
            if (g_numLedsChanged) {
                FastLED[0].setLeds(leds, numLeds);
                FastLED.clear();
                g_numLedsChanged = false;
            }
            g_settingsChanged = false;
        }

        if (!g_isSystemOff) {
            switch (g_mode) {
            case Effects::RAINBOW: Effects::rainbow(leds, numLeds); break;
            case Effects::CYLON: Effects::cylon(leds, numLeds); break;
            case Effects::SPARKLE: Effects::sparkle(leds, numLeds); break;
            case Effects::FIRE: Effects::fire(leds, numLeds); break;
            case Effects::CONFETTI: Effects::confetti(leds, numLeds); break;
            case Effects::SINELON: Effects::sinelon(leds, numLeds); break;
            case Effects::JUGGLE: Effects::juggle(leds, numLeds); break;
            case Effects::BPM: Effects::bpm(leds, numLeds); break;
            case Effects::SNOW: Effects::snow(leds, numLeds); break;
            case Effects::COMET: Effects::comet(leds, numLeds); break;
            case Effects::RAINBOW_GLITTER: Effects::rainbow_glitter(leds, numLeds); break;
            case Effects::COLOR_WAVES: Effects::color_waves(leds, numLeds); break;
            case Effects::THEATER_CHASE: Effects::theater_chase(leds, numLeds); break;
            case Effects::SOLID_GLOW: Effects::solid_glow(leds, numLeds); break;
            default: break;
            }
            FastLED.show();
        } else {
            fill_solid(leds, numLeds, CRGB::Black);
            FastLED.show();
        }
    }

    void effectsTask(void *pvParameters) {
        for (;;) {
            handle_internal();
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }

    void init(int count) {
        numLeds = constrain(count, 1, 256);
        FastLED.addLeds<WS2812B, Pins::STRIP, GRB>(leds, numLeds);
        FastLED.setBrightness(brightness);

        xTaskCreatePinnedToCore(
            effectsTask,
            "EffectsTask",
            4096,
            NULL,
            1,
            NULL,
            0 // Pin to Core 0
        );
        g_useTask = true;
    }
    void setBrightness(int value) {
        brightness = constrain(value, 0, 255);
        g_settingsChanged = true;
    }
    void setNumLeds(int value) {
        numLeds = constrain(value, 1, 256);
        g_numLedsChanged = true;
        g_settingsChanged = true;
    }

    void setMode(Effects::Mode mode) {
        g_mode = mode;
        if (!g_useTask) {
            handle_internal();
        }
    }

    void setSystemOff(bool isSystemOff) {
        g_isSystemOff = isSystemOff;
        if (!g_useTask) {
            handle_internal();
        }
    }
}
