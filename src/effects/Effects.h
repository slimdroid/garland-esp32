#pragma once

#include <FastLED.h>

namespace Effects {
    extern int numLeds;
    extern int brightness;
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
    void setMode(Mode mode);
    void setSystemOff(bool isSystemOff);
    void setBrightness(int value);
    void setNumLeds(int value);
}
