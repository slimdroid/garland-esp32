#include "Effects.h"
#include <Arduino.h>
#include "../board/BoardSelector.h"

namespace Effects {
    int numLeds = 30;       // Number of LEDs (default)
    int brightness = 51;    // Brightness: 20% of 255 (default)
    CRGB leds[256];         // Maximum buffer for LEDs

    static Mode volatile g_mode = RAINBOW;
    static bool volatile g_isSystemOff = false;
    static bool volatile g_settingsChanged = false;
    static bool g_useTask = false;

    void rainbow();
    void cylon();
    void sparkle();
    void fire();
    void confetti();
    void sinelon();
    void juggle();
    void bpm();
    void snow();
    void comet();
    void rainbow_glitter();
    void color_waves();
    void theater_chase();
    void solid_glow();

    void handle_internal() {
        if (g_settingsChanged) {
            FastLED.setBrightness(brightness);
            FastLED[0].setLeds(leds, numLeds);
            FastLED.clear();
            g_settingsChanged = false;
        }

        if (!g_isSystemOff) {
            switch (g_mode) {
            case RAINBOW: rainbow(); break;
            case CYLON: cylon(); break;
            case SPARKLE: sparkle(); break;
            case FIRE: fire(); break;
            case CONFETTI: confetti(); break;
            case SINELON: sinelon(); break;
            case JUGGLE: juggle(); break;
            case BPM: bpm(); break;
            case SNOW: snow(); break;
            case COMET: comet(); break;
            case RAINBOW_GLITTER: rainbow_glitter(); break;
            case COLOR_WAVES: color_waves(); break;
            case THEATER_CHASE: theater_chase(); break;
            case SOLID_GLOW: solid_glow(); break;
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

    void init() {
        FastLED.addLeds<WS2812B, Pins::STRIP, GRB>(leds, numLeds);
        FastLED.setBrightness(brightness);

#if !CONFIG_FREERTOS_UNICORE
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
#endif
    }
    void setBrightness(int value) {
        brightness = constrain(value, 0, 255);
        g_settingsChanged = true;
    }
    void setNumLeds(int value) {
        numLeds = constrain(value, 1, 256);
        g_settingsChanged = true;
    }

    void rainbow() {
        static uint8_t hue = 0;
        fill_rainbow(leds, numLeds, hue++, 7);
    }

    void cylon() {
        static uint8_t i = 0;
        static bool forward = true;
        
        fadeToBlackBy(leds, numLeds, 20);
        leds[i] = CHSV(millis() / 10, 255, 255);
        
        if (forward) {
            i++;
            if (i >= numLeds - 1) forward = false;
        } else {
            i--;
            if (i == 0) forward = true;
        }
    }

    void sparkle() {
        fadeToBlackBy(leds, numLeds, 10);
        if (random8() < 30) {
            leds[random16(numLeds)] = CRGB::White;
        }
    }

    void fire() {
        // Simplified fire effect
        for (int i = 0; i < numLeds; i++) {
            uint8_t noise = qsub8(inoise8(i * 60, millis() / 4), 16);
            leds[i] = CHSV(10 + (noise / 8), 255, noise);
        }
    }

    void confetti() {
        fadeToBlackBy(leds, numLeds, 10);
        int pos = random16(numLeds);
        leds[pos] += CHSV(random8(255), 200, 255);
    }

    void sinelon() {
        fadeToBlackBy(leds, numLeds, 20);
        int pos = beatsin16(13, 0, numLeds - 1);
        leds[pos] += CHSV(millis() / 20, 255, 192);
    }

    void juggle() {
        fadeToBlackBy(leds, numLeds, 20);
        uint8_t dothue = 0;
        for (int i = 0; i < 8; i++) {
            leds[beatsin16(i + 7, 0, numLeds - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }

    void bpm() {
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < numLeds; i++) {
            leds[i] = ColorFromPalette(palette, (millis() / 10) + (i * 2), beat - (millis() / 10) + (i * 10));
        }
    }

    void snow() {
        fadeToBlackBy(leds, numLeds, 20);
        if (random8() < 20) {
            leds[random16(numLeds)] = CRGB::White;
        }
    }

    void comet() {
        static uint16_t pos = 0;
        fadeToBlackBy(leds, numLeds, 40);
        leds[pos % numLeds] = CHSV(millis() / 10, 255, 255);
        pos++;
    }

    void rainbow_glitter() {
        rainbow();
        if (random8() < 80) {
            leds[random16(numLeds)] += CRGB::White;
        }
    }

    void color_waves() {
        uint8_t hue = millis() / 50;
        for (int i = 0; i < numLeds; i++) {
            int colorIndex = (hue + (i * 255 / numLeds)) % 255;
            leds[i] = CHSV(colorIndex, 255, beatsin8(10, 160, 255, 0, i * 10));
        }
    }

    void theater_chase() {
        static uint8_t frame = 0;
        fadeToBlackBy(leds, numLeds, 100);
        for (int i = frame; i < numLeds; i += 3) {
            leds[i] = CHSV(millis() / 20, 255, 255);
        }
        frame = (frame + 1) % 3;
    }

    void solid_glow() {
        fill_solid(leds, numLeds, CHSV(millis() / 50, 255, beatsin8(15, 100, 255)));
    }

    void setMode(Mode mode) {
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
