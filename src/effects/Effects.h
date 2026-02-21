#pragma once

#include <FastLED.h>

namespace Effects {
    extern const int NUM_LEDS;
    extern CRGB leds[];

    enum Mode {
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

    void init();
    void handle(Mode mode, bool isSystemOff);

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
}
