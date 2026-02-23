#include <FastLED.h>

namespace Effects {
    void rainbow(CRGB *leds, int numLeds) {
        static uint8_t hue = 0;
        fill_rainbow(leds, numLeds, hue++, 7);
    }

    void cylon(CRGB *leds, int numLeds) {
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

    void sparkle(CRGB *leds, int numLeds) {
        fadeToBlackBy(leds, numLeds, 10);
        if (random8() < 30) {
            leds[random16(numLeds)] = CRGB::White;
        }
    }

    void fire(CRGB *leds, int numLeds) {
        // Simplified fire effect
        for (int i = 0; i < numLeds; i++) {
            uint8_t noise = qsub8(inoise8(i * 60, millis() / 4), 16);
            leds[i] = CHSV(10 + (noise / 8), 255, noise);
        }
    }

    void confetti(CRGB *leds, int numLeds) {
        fadeToBlackBy(leds, numLeds, 10);
        int pos = random16(numLeds);
        leds[pos] += CHSV(random8(255), 200, 255);
    }

    void sinelon(CRGB *leds, int numLeds) {
        fadeToBlackBy(leds, numLeds, 20);
        int pos = beatsin16(13, 0, numLeds - 1);
        leds[pos] += CHSV(millis() / 20, 255, 192);
    }

    void juggle(CRGB *leds, int numLeds) {
        fadeToBlackBy(leds, numLeds, 20);
        uint8_t dothue = 0;
        for (int i = 0; i < 8; i++) {
            leds[beatsin16(i + 7, 0, numLeds - 1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }

    void bpm(CRGB *leds, int numLeds) {
        uint8_t BeatsPerMinute = 62;
        CRGBPalette16 palette = PartyColors_p;
        uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
        for (int i = 0; i < numLeds; i++) {
            leds[i] = ColorFromPalette(palette, (millis() / 10) + (i * 2), beat - (millis() / 10) + (i * 10));
        }
    }

    void snow(CRGB *leds, int numLeds) {
        fadeToBlackBy(leds, numLeds, 20);
        if (random8() < 20) {
            leds[random16(numLeds)] = CRGB::White;
        }
    }

    void comet(CRGB *leds, int numLeds) {
        static uint16_t pos = 0;
        fadeToBlackBy(leds, numLeds, 40);
        leds[pos % numLeds] = CHSV(millis() / 10, 255, 255);
        pos++;
    }

    void rainbow_glitter(CRGB *leds, int numLeds) {
        rainbow(leds, numLeds);
        if (random8() < 80) {
            leds[random16(numLeds)] += CRGB::White;
        }
    }

    void color_waves(CRGB *leds, int numLeds) {
        uint8_t hue = millis() / 50;
        for (int i = 0; i < numLeds; i++) {
            int colorIndex = (hue + (i * 255 / numLeds)) % 255;
            leds[i] = CHSV(colorIndex, 255, beatsin8(10, 160, 255, 0, i * 10));
        }
    }

    void theater_chase(CRGB *leds, int numLeds) {
        static uint8_t frame = 0;
        fadeToBlackBy(leds, numLeds, 100);
        for (int i = frame; i < numLeds; i += 3) {
            leds[i] = CHSV(millis() / 20, 255, 255);
        }
        frame = (frame + 1) % 3;
    }

    void solid_glow(CRGB *leds, int numLeds) {
        fill_solid(leds, numLeds, CHSV(millis() / 50, 255, beatsin8(15, 100, 255)));
    }
}