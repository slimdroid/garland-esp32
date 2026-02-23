#include <Preferences.h>
#include "Settings.h"

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
#define KEY_BRIGHTNESS "brightness"
#define KEY_BRIGHTNESS_DEF 51
#define KEY_NUM_LEDS "numLeds"
#define KEY_NUM_LEDS_DEF 60

#define DEBOUNCE_MS 5000

namespace Settings {
    static const char *TAG = "SETTINGS";

    static Preferences preferences;
    static int cachedMode = KEY_MODE_DEF;
    static int cachedBrightness = KEY_BRIGHTNESS_DEF;
    static bool modeDirty = false;
    static unsigned long lastModeChange = 0;
    static unsigned long brightnessChange = 0;

    void saveLightModeNow() {
        preferences.begin(PREF_LIGHT, false);
        preferences.putInt(KEY_MODE, cachedMode);
        preferences.end();
        modeDirty = false;
        lastModeChange = 0;
        ESP_LOGI(TAG, "Light mode %d saved to NVS", cachedMode);
    }

    void saveBrightnessNow() {
        preferences.begin(PREF_LIGHT, false);
        preferences.putInt(KEY_BRIGHTNESS, cachedBrightness);
        preferences.end();
        modeDirty = false;
        lastModeChange = 0;
        ESP_LOGI(TAG, "Brightness %d saved to NVS", cachedBrightness);
    }

    bool getWiFiCredentials(String &ssid, String &password) {
        preferences.begin(PREF_NAME, true);
        ssid = preferences.getString(KEY_SSID, KEY_SSID_DEF);
        password = preferences.getString(KEY_PASS, KEY_PASS_DEF);
        preferences.end();
        ESP_LOGI(TAG,
            "Get network credentials - SSID: %s, PASSWORD: %s",
            ssid.c_str(),
            password.c_str());
        return !ssid.isEmpty();
    }

    void setWiFiCredentials(const String &ssid, const String &password) {
        preferences.begin(PREF_NAME, false);
        preferences.putString(KEY_SSID, ssid);
        preferences.putString(KEY_PASS, password);
        preferences.end();
        ESP_LOGI(TAG,
            "Network credentials have been saved - SSID: %s, PASSWORD: %s",
            ssid.c_str(),
            password.c_str());
    }

    void saveLightMode(const int mode) {
        cachedMode = mode;
        modeDirty = true;
        lastModeChange = millis();
        ESP_LOGD(TAG, "saveLightMode: mode=%d", mode);
    }

    void saveSystemState(const bool isOff) {
        preferences.begin(PREF_LIGHT, false);
        preferences.putBool(KEY_OFF, isOff);
        preferences.end();
        ESP_LOGD(TAG, "saveSystemState: isOff=%d", isOff);

        if (isOff && modeDirty) {
            saveLightModeNow();
            saveBrightnessNow();
        }
    }

    void saveBrightness(const int brightness) {
        brightnessChange = brightness;
        modeDirty = true;
        lastModeChange = millis();
        ESP_LOGD(TAG, "saveBrightness: brightness=%d", brightness);
    }
    void saveNumLeds(const int numLeds) {
        preferences.begin(PREF_LIGHT, false);
        preferences.putInt(KEY_NUM_LEDS, numLeds);
        preferences.end();
        ESP_LOGD(TAG, "saveNumLeds: numLeds=%d", numLeds);
    }
    void loadLightSettings(int &mode, bool &isOff, int &brightness, int &numLeds) {
        preferences.begin(PREF_LIGHT, true);
        mode = preferences.getInt(KEY_MODE, KEY_MODE_DEF);
        isOff = preferences.getBool(KEY_OFF, KEY_OFF_DEF);
        brightness = preferences.getInt(KEY_BRIGHTNESS, KEY_BRIGHTNESS_DEF);
        numLeds = preferences.getInt(KEY_NUM_LEDS, KEY_NUM_LEDS_DEF);
        preferences.end();
        ESP_LOGI(TAG, "loadLightSettings: mode=%d, isOff=%d, brightness=%d, numLeds=%d", mode, isOff, brightness, numLeds);
    }

    void handleSettingsSync() {
        if (modeDirty && (millis() - lastModeChange >= DEBOUNCE_MS)) {
            saveLightModeNow();
            saveBrightnessNow();
        }
    }
}
