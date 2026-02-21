#include <Arduino.h>
#include "DataParser.h"
#include "../settings/Settings.h"
#include "../effects/Effects.h"

namespace DataParser {
    static const char *TAG = "PARSER";

    // Global variables for state management (must be passed from outside)
    extern Effects::Mode *currentMode;
    extern bool *isSystemOff;

    bool parse(const String &data) {
        String input = data;
        input.trim();
        ESP_LOGD(TAG, "DATA: %s ", input.c_str());

        // Simple parsing for JSON-like format
        // Supports: set_mode, set_power, get_status, set_wifi
        
        if (input.indexOf("\"cmd\":\"set_mode\"") != -1) {
            int modeIdx = input.indexOf("\"mode\":");
            if (modeIdx != -1) {
                int valStart = modeIdx + 7;
                int valEnd = input.indexOf(',', valStart);
                if (valEnd == -1) valEnd = input.indexOf('}', valStart);
                if (valEnd == -1) valEnd = input.length();
                
                int mode = input.substring(valStart, valEnd).toInt();
                if (mode >= 0 && mode < Effects::NUM_MODES) {
                    if (currentMode) {
                        *currentMode = static_cast<Effects::Mode>(mode);
                        Settings::saveLightMode(mode);
                        ESP_LOGI(TAG, "Mode set to: %d", mode);
                        return true;
                    }
                }
            }
        }
        else if (input.indexOf("\"cmd\":\"set_power\"") != -1) {
            int stateIdx = input.indexOf("\"state\":");
            if (stateIdx != -1) {
                int valStart = stateIdx + 8;
                int valEnd = input.indexOf(',', valStart);
                if (valEnd == -1) valEnd = input.indexOf('}', valStart);
                if (valEnd == -1) valEnd = input.length();
                
                int state = input.substring(valStart, valEnd).toInt();
                if (isSystemOff) {
                    *isSystemOff = (state == 0);
                    Settings::saveSystemState(*isSystemOff);
                    ESP_LOGI(TAG, "System power set to: %s", *isSystemOff ? "OFF" : "ON");
                    return true;
                }
            }
        }
        else if (input.indexOf("\"cmd\":\"get_status\"") != -1) {
            if (currentMode && isSystemOff) {
                ESP_LOGI(TAG, "Status - Mode: %d, Power: %s", *currentMode, *isSystemOff ? "OFF" : "ON");
                return true;
            }
        }
        else if (input.indexOf("\"cmd\":\"set_wifi\"") != -1) {
            auto extractField = [&](const String& key) -> String {
                int keyIdx = input.indexOf("\"" + key + "\":\"");
                if (keyIdx == -1) return "";
                int valStart = keyIdx + key.length() + 4;
                int valEnd = input.indexOf("\"", valStart);
                return (valEnd != -1) ? input.substring(valStart, valEnd) : "";
            };
            
            String ssid = extractField("ssid");
            String pass = extractField("pass");
            
            if (ssid.length() > 0) {
                Settings::setWiFiCredentials(ssid, pass);
                ESP_LOGI(TAG, "WiFi credentials saved - SSID: %s", ssid.c_str());
                return true;
            }
        }
        
        ESP_LOGW(TAG, "ERROR: Parsing failed or unknown command");
        return false;
    }
}
