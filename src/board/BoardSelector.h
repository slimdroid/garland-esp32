#pragma once

#if defined(BOARD_ESP32_DEVKIT)
    #include "esp32devkit/Pins.h"
#elif defined(BOARD_M5_NANOC6)
    #include "m5nanoc6/Pins.h"
#else
    #error "Board not defined!"
#endif