#pragma once

namespace Settings {
    bool getWiFiCredentials(String &ssid, String &password);

    void setWiFiCredentials(const String &ssid, const String &password);

    void saveLightMode(int mode);

    void saveSystemState(bool isOff);

    void loadLightSettings(int &mode, bool &isOff);

    void handleSettingsSync();
}
