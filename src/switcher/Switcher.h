#pragma once

#include "../effects/Effects.h"

namespace Switcher {

    void init(int count);
    void setMode(Effects::Mode mode);
    void setSystemOff(bool isSystemOff);
    void setBrightness(int value);
    void setNumLeds(int value);
}
