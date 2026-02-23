#include <Arduino.h>
#include <ArduinoJson.h>
#include "DataParser.h"
#include "../settings/Settings.h"
#include "../switcher/Switcher.h"
#include "../effects/Effects.h"

namespace DataParser {
    static const char *TAG = "PARSER";

    static Effects::Mode *s_currentMode = nullptr;
    static bool *s_isSystemOff = nullptr;

    void setContext(Effects::Mode *mode, bool *isSystemOff) {
        s_currentMode = mode;
        s_isSystemOff = isSystemOff;
    }

    bool parse(const String &data) {
        String input = data;
        input.trim();
        ESP_LOGD(TAG, "DATA: %s", input.c_str());

        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, input);
        if (err) {
            ESP_LOGW(TAG, "JSON parse error: %s", err.c_str());
            return false;
        }

        const char *cmd = doc["cmd"];
        if (!cmd) {
            ESP_LOGW(TAG, "ERROR: Missing 'cmd' field");
            return false;
        }

        if (strcmp(cmd, "set_mode") == 0) {
            if (!s_currentMode) return false;
            int mode = doc["mode"] | -1;
            if (mode >= 0 && mode < Effects::NUM_MODES) {
                *s_currentMode = static_cast<Effects::Mode>(mode);
                Settings::saveLightMode(mode);
                ESP_LOGI(TAG, "Mode set to: %d", mode);
                return true;
            }
            ESP_LOGW(TAG, "Invalid mode value: %d", mode);
            return false;
        }

        if (strcmp(cmd, "set_power") == 0) {
            if (!s_isSystemOff) return false;
            int state = doc["state"] | -1;
            if (state == 0 || state == 1) {
                *s_isSystemOff = (state == 0);
                Settings::saveSystemState(*s_isSystemOff);
                ESP_LOGI(TAG, "System power set to: %s", *s_isSystemOff ? "OFF" : "ON");
                return true;
            }
            ESP_LOGW(TAG, "Invalid state value: %d", state);
            return false;
        }

        if (strcmp(cmd, "get_status") == 0) {
            if (s_currentMode && s_isSystemOff) {
                ESP_LOGI(TAG, "Status - Mode: %d, Power: %s", *s_currentMode, *s_isSystemOff ? "OFF" : "ON");
                return true;
            }
            return false;
        }

        if (strcmp(cmd, "set_brightness") == 0) {
            int value = doc["value"] | -1;
            if (value < 0 || value > 255) {
                ESP_LOGW(TAG, "Invalid brightness value: %d", value);
                return false;
            }
            Switcher::setBrightness(value);
            Settings::saveBrightness(value);
            ESP_LOGI(TAG, "Brightness set to: %d", value);
            return true;
        }

        if (strcmp(cmd, "set_led_count") == 0) {
            int value = doc["value"] | -1;
            if (value < 1 || value > 256) {
                ESP_LOGW(TAG, "Invalid LED count: %d", value);
                return false;
            }
            Switcher::setNumLeds(value);
            Settings::saveNumLeds(value);
            ESP_LOGI(TAG, "LED count set to: %d", value);
            return true;
        }

        if (strcmp(cmd, "set_wifi") == 0) {
            const char *ssid = doc["ssid"];
            const char *pass = doc["pass"] | "";
            if (ssid && strlen(ssid) > 0) {
                Settings::setWiFiCredentials(String(ssid), String(pass));
                ESP_LOGI(TAG, "WiFi credentials saved - SSID: %s", ssid);
                return true;
            }
            ESP_LOGW(TAG, "Missing or empty SSID");
            return false;
        }

        ESP_LOGW(TAG, "ERROR: Unknown command: %s", cmd);
        return false;
    }
}
