#include <Preferences.h>
#include "Settings.h"
#include "Logging.h"

#define PREF_NAME "wifi-settings"
#define KEY_SSID "ssid"
#define KEY_SSID_DEF ""
#define KEY_PASS "password"
#define KEY_PASS_DEF ""

#define PREF_LIGHT "light-settings"
#define KEY_MODE "mode"
#define KEY_MODE_DEF 0
#define KEY_OFF "systemOff"
#define KEY_OFF_DEF false

#define DEBOUNCE_MS 5000

namespace Settings {
    static const char *TAG = "SETTINGS";

    static Preferences preferences;
    static int cachedMode = KEY_MODE_DEF;
    static bool modeDirty = false;
    static unsigned long lastModeChange = 0;

    static void saveLightModeNow() {
        preferences.begin(PREF_LIGHT, false);
        preferences.putInt(KEY_MODE, cachedMode);
        preferences.end();
        modeDirty = false;
        LOG(TAG, "Light mode %d saved to NVS", cachedMode);
    }

    bool getWiFiCredentials(String &ssid, String &password) {
        preferences.begin(PREF_NAME, true);
        ssid = preferences.getString(KEY_SSID, KEY_SSID_DEF);
        password = preferences.getString(KEY_PASS, KEY_PASS_DEF);
        preferences.end();
        LOG(TAG,
            "Get network credentials - SSID: %s, PASSWORD: %s",
            ssid.c_str(),
            password.c_str());
        return ssid != KEY_SSID_DEF && password != KEY_PASS_DEF;
    }

    void setWiFiCredentials(const String &ssid, const String &password) {
        preferences.begin(PREF_NAME, false);
        preferences.putString(KEY_SSID, ssid);
        preferences.putString(KEY_PASS, password);
        preferences.end();
        LOG(TAG,
            "Network credentials have been saved - SSID: %s, PASSWORD: %s",
            ssid.c_str(),
            password.c_str());
    }

    void saveLightMode(const int mode) {
        cachedMode = mode;
        modeDirty = true;
        lastModeChange = millis();
        LOG(TAG, "saveLightMode: mode=%d", mode);
    }

    void saveSystemState(const bool isOff) {
        preferences.begin(PREF_LIGHT, false);
        preferences.putBool(KEY_OFF, isOff);
        preferences.end();
        LOG(TAG, "saveSystemState: isOff=%d", isOff);

        if (isOff && modeDirty) saveLightModeNow();
    }

    void loadLightSettings(int &mode, bool &isOff) {
        preferences.begin(PREF_LIGHT, true);
        mode = preferences.getInt(KEY_MODE, KEY_MODE_DEF);
        isOff = preferences.getBool(KEY_OFF, KEY_OFF_DEF);
        preferences.end();
        LOG(TAG, "loadLightSettings: mode=%d, isOff=%d", mode, isOff);
    }

    void handleSettingsSync() {
        if (modeDirty && (millis() - lastModeChange >= DEBOUNCE_MS)) saveLightModeNow();
    }
}
