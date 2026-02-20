#include <Arduino.h>
#include "DataParser.h"
#include "../Logging.h"
#include "../settings/Settings.h"
#include "../led_strip/StringLED.h"

namespace DataParser {
    static const char *TAG = "PARSER";

    // Global variables for state management (must be passed from outside)
    extern StringLED::LightMode *currentMode;
    extern bool *isSystemOff;

    bool parse(const String &data) {
        String input = data;
        input.trim();
        LOG(TAG, "DATA: %s ", input.c_str());

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
                if (mode >= 0 && mode < StringLED::NUM_MODES) {
                    if (currentMode) {
                        *currentMode = static_cast<StringLED::LightMode>(mode);
                        Settings::saveLightMode(mode);
                        LOG(TAG, "Mode set to: %d", mode);
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
                    LOG(TAG, "System power set to: %s", *isSystemOff ? "OFF" : "ON");
                    return true;
                }
            }
        }
        else if (input.indexOf("\"cmd\":\"get_status\"") != -1) {
            if (currentMode && isSystemOff) {
                LOG(TAG, "Status - Mode: %d, Power: %s", *currentMode, *isSystemOff ? "OFF" : "ON");
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
                LOG(TAG, "WiFi credentials saved - SSID: %s", ssid.c_str());
                return true;
            }
        }
        
        LOG(TAG, "ERROR: Parsing failed or unknown command");
        return false;
    }
}
