#pragma once

#include "bluetooth/Bluetooth.h"

/**
 * @brief Interface for Bluetooth connection indicator
 */
class IBtIndicator {
public:
    virtual ~IBtIndicator() = default;

    /**
     * @brief Updates indicator state based on BT connection state
     * @param state Current Bluetooth connection state
     */
    virtual void setState(BT_ConnectionState state) = 0;

    /**
     * @brief Periodic handler (call in loop())
     */
    virtual void handle() = 0;
};
