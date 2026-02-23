#pragma once

#include <FastLED.h>
#include "../effects/Effects.h"

namespace Switcher {
    extern int numLeds;
    extern int brightness;
    extern CRGB leds[];

    void init();
    void setMode(Effects::Mode mode);
    void setSystemOff(bool isSystemOff);
    void setBrightness(int value);
    void setNumLeds(int value);
}
