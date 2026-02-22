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
     * @brief Saves the brightness level
     * @param brightness Brightness value (0–255)
     */
    void saveBrightness(int brightness);
    /**
     * @brief Saves the number of LEDs
     * @param numLeds Number of LEDs
     */
    void saveNumLeds(int numLeds);
    /**
     * @brief Loads lighting settings from persistent storage
     * @param mode Output for light mode ID
     * @param isOff Output for device on/off state
     * @param brightness Output for brightness value (0–255)
     * @param numLeds Output for number of LEDs
     */
    void loadLightSettings(int &mode, bool &isOff, int &brightness, int &numLeds);

    /**
     * @brief Handles settings synchronization (e.g., commit changes to storage)
     */
    void handleSettingsSync();
}
