#pragma once

#include <FastLED.h>

namespace Effects
{
    enum Mode
    {
        RAINBOW,
        CYLON,
        SPARKLE,
        FIRE,
        CONFETTI,
        SINELON,
        JUGGLE,
        BPM,
        SNOW,
        COMET,
        RAINBOW_GLITTER,
        COLOR_WAVES,
        THEATER_CHASE,
        SOLID_GLOW,
        NUM_MODES
    };

    void rainbow(CRGB* leds, int numLeds);
    void cylon(CRGB* leds, int numLeds);
    void sparkle(CRGB* leds, int numLeds);
    void fire(CRGB* leds, int numLeds);
    void confetti(CRGB* leds, int numLeds);
    void sinelon(CRGB* leds, int numLeds);
    void juggle(CRGB* leds, int numLeds);
    void bpm(CRGB* leds, int numLeds);
    void snow(CRGB* leds, int numLeds);
    void comet(CRGB* leds, int numLeds);
    void rainbow_glitter(CRGB* leds, int numLeds);
    void color_waves(CRGB* leds, int numLeds);
    void theater_chase(CRGB* leds, int numLeds);
    void solid_glow(CRGB* leds, int numLeds);
}
