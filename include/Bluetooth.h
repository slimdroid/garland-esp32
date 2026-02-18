#pragma once

#include <Arduino.h>

/**
 * @brief Bluetooth connection state enumeration
 */
enum BT_ConnectionState {
    BT_ENABLED,
    BT_CONNECTED,
    BT_DISCONNECTED,
    BT_DISABLED
};

/**
 * @brief Callback type for receiving credentials via BLE
 * @param value String containing the received data
 */
typedef void (*BluetoothCredentialsReceivedCallback)(String value);

/**
 * @brief Callback type for BLE connection state changes
 * @param value New connection state
 */
typedef void (*BluetoothConnectionStateCallback)(BT_ConnectionState value);

namespace Bluetooth {
    /**
     * @brief Initializes the BLE module and system handlers (Wi-Fi events, etc.)
     * @param credentialsCallback Function called when the credentials characteristic is written
     * @param stateCallback Function called when the connection state changes
     */
    void init(
        BluetoothCredentialsReceivedCallback credentialsCallback,
        BluetoothConnectionStateCallback stateCallback
    );

    /**
     * @brief Enables BLE mode (creates server/services/characteristics and starts advertising)
     */
    void enable();

    /**
     * @brief Disables BLE mode
     */
    void disable();

    /**
     * @brief Sends device uptime via BLE
     * @param seconds Time in seconds
     */
    void sendWorkedTime(int seconds);

    /**
     * @brief Checks if BLE is currently connected
     * @return true if a device is connected via BLE
     */
    bool isConnected();

    /**
     * @brief Sends Wi-Fi connection status to the connected BLE client
     * @param success Connection success flag
     * @param value Optional message or status info
     */
    void sendWiFiConnectInfo(bool success, const String &value);
}
