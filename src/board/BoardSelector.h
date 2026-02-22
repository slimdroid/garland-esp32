#pragma once

#if defined(BOARD_ESP32_DEVKIT)
    #include "esp32devkit/Pins.h"
    #include "bt_indicator/LedBtIndicator.h"
    using BtIndicator = LedBtIndicator<Pins::LED>;
#elif defined(BOARD_M5_NANOC6)
    #include "m5nanoc6/Pins.h"
    #include "bt_indicator/RgbLedBtIndicator.h"
    using BtIndicator = RgbLedBtIndicator<Pins::LED>;
#elif defined(BOARD_M5_ATOMS3)
    #include "m5atoms3/Pins.h"
    #include "bt_indicator/RgbLedBtIndicator.h"
    using BtIndicator = RgbLedBtIndicator<Pins::LED>;
#else
    #error "Board not defined!"
#endif
