#pragma once

/**
 * @brief Management of persistent device settings
 */
namespace Settings {
    /**
     * @brief Loads Wi-Fi credentials from persistent storage
     * @param ssid Output string for SSID
     * @param password Output string for password
     * @return true if credentials were successfully loaded
     */
    bool getWiFiCredentials(String &ssid, String &password);

    /**
     * @brief Saves Wi-Fi credentials to persistent storage
     * @param ssid SSID to save
     * @param password Password to save
     */
    void setWiFiCredentials(const String &ssid, const String &password);

    /**
     * @brief Saves the current lighting mode
     * @param mode Light mode ID
     */
    void saveLightMode(int mode);

    /**
     * @brief Saves the device on/off state
     * @param isOff true if the device is turned off
     */
    void saveSystemState(bool isOff);

    /**
     * @brief Loads lighting settings from persistent storage
     * @param mode Output for light mode ID
     * @param isOff Output for device on/off state
     */
    void loadLightSettings(int &mode, bool &isOff);

    /**
     * @brief Handles settings synchronization (e.g., commit changes to storage)
     */
    void handleSettingsSync();
}
