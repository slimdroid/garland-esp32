#pragma once

/**
 * @brief Callback type for Wi-Fi connection status changes
 * @param connected true if connected to Wi-Fi
 * @param message Status message or assigned IP address
 */
typedef void (*WifiStatusCallback)(bool connected, const String &message);

/**
 * @brief Management of Wi-Fi connection and reconnection logic
 */
namespace WiFiManager {
    /**
     * @brief Initializes the Wi-Fi manager with a status callback
     * @param callback Callback function for connection updates
     */
    void init(WifiStatusCallback callback);

    /**
     * @brief Connects to a Wi-Fi network with given credentials
     * @param ssid Network SSID
     * @param password Network password
     */
    void connect(const String &ssid, const String &password);

    /**
     * @brief Handles automatic reconnection if the connection is lost
     */
    void handleReconnect();

    /**
     * @brief Checks if device is currently connected to Wi-Fi
     * @return true if connected
     */
    bool isConnected();
}
