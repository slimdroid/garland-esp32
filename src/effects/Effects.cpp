#include "Effects.h"
#include "../board/BoardSelector.h"

namespace Effects {
    const int NUM_LEDS = 30; // Настройте количество светодиодов под вашу ленту
    const int BRIGHTNESS = 51;  // 20% от 255

    CRGB leds[NUM_LEDS];

    void init() {
        FastLED.addLeds<WS2812B, Pins::STRIP, GRB>(leds, NUM_LEDS);
        FastLED.setBrightness(BRIGHTNESS);
    }

    void handle(Mode mode, bool isSystemOff) {
        if (!isSystemOff) {
            switch (mode) {
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
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
        }
    }

    void rainbow() {
        static uint8_t hue = 0;
        fill_rainbow(leds, NUM_LEDS, hue++, 7);
    }

    void cylon() {
        static uint8_t i = 0;
        static bool forward = true;
        
        fadeToBlackBy(leds, NUM_LEDS, 20);
        leds[i] = CHSV(millis() / 10, 255, 255);
        
        if (forward) {
            i++;
            if (i >= NUM_LEDS - 1) forward = false;
        } else {
            i--;
            if (i == 0) forward = true;
        }
    }

    void sparkle() {
        fadeToBlackBy(leds, NUM_LEDS, 10);
        if (random8() < 30) {
            leds[random16(NUM_LEDS)] = CRGB::White;
        }
    }

    void fire() {
        // Упрощенный эффект огня
        for (int i = 0; i < NUM_LEDS; i++) {
            uint8_t noise = qsub8(inoise8(i * 60, millis() / 4), 16);
            leds[i] = CHSV(10 + (noise / 8), 255, noise);
        }
    }

    void confetti() {
        fadeToBlackBy(leds, NUM_LEDS, 10);
        int pos = random16(NUM_LEDS);
        leds[pos] += CHSV(random8(255), 200, 255);
    }

    void sinelon() {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        int pos = beatsin16(13, 0, NUM_LEDS - 1);
        leds[pos] += CHSV(millis() / 20, 255, 192);
    }

    void juggle() {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        uint8_t dothue = 0;
        for (int i = 0; i < 8; i++) {
            leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }

    void bpm() {
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = ColorFromPalette(palette, (millis() / 10) + (i * 2), beat - (millis() / 10) + (i * 10));
        }
    }

    void snow() {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        if (random8() < 20) {
            leds[random16(NUM_LEDS)] = CRGB::White;
        }
    }

    void comet() {
        static uint16_t pos = 0;
        fadeToBlackBy(leds, NUM_LEDS, 40);
        leds[pos % NUM_LEDS] = CHSV(millis() / 10, 255, 255);
        pos++;
    }

    void rainbow_glitter() {
        rainbow();
        if (random8() < 80) {
            leds[random16(NUM_LEDS)] += CRGB::White;
        }
    }

    void color_waves() {
        uint8_t hue = millis() / 50;
        for (int i = 0; i < NUM_LEDS; i++) {
            int colorIndex = (hue + (i * 255 / NUM_LEDS)) % 255;
            leds[i] = CHSV(colorIndex, 255, beatsin8(10, 160, 255, 0, i * 10));
        }
    }

    void theater_chase() {
        static uint8_t frame = 0;
        fadeToBlackBy(leds, NUM_LEDS, 100);
        for (int i = frame; i < NUM_LEDS; i += 3) {
            leds[i] = CHSV(millis() / 20, 255, 255);
        }
        frame = (frame + 1) % 3;
    }

    void solid_glow() {
        fill_solid(leds, NUM_LEDS, CHSV(millis() / 50, 255, beatsin8(15, 100, 255)));
    }
}
