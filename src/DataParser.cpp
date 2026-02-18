#include <Arduino.h>
#include "DataParser.h"
#include "Logging.h"
#include "Settings.h"
#include "StringLED.h"

namespace DataParser {
    static const char *TAG = "PARSER";

    // Global variables for state management (must be passed from outside)
    extern StringLED::LightMode *currentMode;
    extern bool *isSystemOff;

    bool parse(const String &data) {
        LOG(TAG, "DATA: %s ", data.c_str());

        // Protocol: JSON-like format with commands
        // Format: {"cmd":"<command>","param":"<value>"}
        // Supported commands:
        // 1. {"cmd":"set_mode","mode":<0-4>} - set LED mode (0=FADE,1=RED,2=GREEN,3=BLUE,4=WHITE)
        // 2. {"cmd":"set_power","state":<0|1>} - turn system on/off (0=OFF,1=ON)
        // 3. {"cmd":"get_status"} - get current status
        // 4. {"cmd":"set_wifi","ssid":"<ssid>","pass":"<password>"} - set WiFi credentials

        // Simple parsing without JSON library
        int cmdStart = data.indexOf("\"cmd\"");
        if (cmdStart == -1) {
            LOG(TAG, "ERROR: No 'cmd' field found");
            return false;
        }

        // Extract command
        int cmdValueStart = data.indexOf(":", cmdStart) + 1;
        int cmdQuoteStart = data.indexOf("\"", cmdValueStart) + 1;
        int cmdQuoteEnd = data.indexOf("\"", cmdQuoteStart);
        
        if (cmdQuoteStart == 0 || cmdQuoteEnd == -1) {
            LOG(TAG, "ERROR: Invalid command format");
            return false;
        }

        String command = data.substring(cmdQuoteStart, cmdQuoteEnd);
        LOG(TAG, "Command: %s", command.c_str());

        // Handle set_mode command
        if (command == "set_mode") {
            int modeStart = data.indexOf("\"mode\"");
            if (modeStart == -1) {
                LOG(TAG, "ERROR: No 'mode' parameter");
                return false;
            }
            
            int modeValueStart = data.indexOf(":", modeStart) + 1;
            int modeValueEnd = data.indexOf(",", modeValueStart);
            if (modeValueEnd == -1) {
                modeValueEnd = data.indexOf("}", modeValueStart);
            }
            
            String modeStr = data.substring(modeValueStart, modeValueEnd);
            modeStr.trim();
            int mode = modeStr.toInt();
            
            if (mode >= 0 && mode < StringLED::NUM_MODES) {
                if (currentMode != nullptr) {
                    *currentMode = static_cast<StringLED::LightMode>(mode);
                    Settings::saveLightMode(mode);
                    LOG(TAG, "Mode set to: %d", mode);
                    return true;
                } else {
                    LOG(TAG, "ERROR: currentMode pointer is null");
                    return false;
                }
            } else {
                LOG(TAG, "ERROR: Invalid mode value: %d", mode);
                return false;
            }
        }
        
        // Handle set_power command
        else if (command == "set_power") {
            int stateStart = data.indexOf("\"state\"");
            if (stateStart == -1) {
                LOG(TAG, "ERROR: No 'state' parameter");
                return false;
            }
            
            int stateValueStart = data.indexOf(":", stateStart) + 1;
            int stateValueEnd = data.indexOf(",", stateValueStart);
            if (stateValueEnd == -1) {
                stateValueEnd = data.indexOf("}", stateValueStart);
            }
            
            String stateStr = data.substring(stateValueStart, stateValueEnd);
            stateStr.trim();
            int state = stateStr.toInt();
            
            if (isSystemOff != nullptr) {
                *isSystemOff = (state == 0);
                Settings::saveSystemState(*isSystemOff);
                LOG(TAG, "System power set to: %s", *isSystemOff ? "OFF" : "ON");
                return true;
            } else {
                LOG(TAG, "ERROR: isSystemOff pointer is null");
                return false;
            }
        }
        
        // Handle get_status command
        else if (command == "get_status") {
            if (currentMode != nullptr && isSystemOff != nullptr) {
                LOG(TAG, "Status - Mode: %d, Power: %s", *currentMode, *isSystemOff ? "OFF" : "ON");
                // Here we can send response back via callback
                return true;
            } else {
                LOG(TAG, "ERROR: State pointers are null");
                return false;
            }
        }
        
        // Handle set_wifi command
        else if (command == "set_wifi") {
            int ssidStart = data.indexOf("\"ssid\"");
            int passStart = data.indexOf("\"pass\"");
            
            if (ssidStart == -1 || passStart == -1) {
                LOG(TAG, "ERROR: Missing 'ssid' or 'pass' parameter");
                return false;
            }
            
            // Extract SSID
            int ssidValueStart = data.indexOf(":", ssidStart) + 1;
            int ssidQuoteStart = data.indexOf("\"", ssidValueStart) + 1;
            int ssidQuoteEnd = data.indexOf("\"", ssidQuoteStart);
            String ssid = data.substring(ssidQuoteStart, ssidQuoteEnd);
            
            // Extract password
            int passValueStart = data.indexOf(":", passStart) + 1;
            int passQuoteStart = data.indexOf("\"", passValueStart) + 1;
            int passQuoteEnd = data.indexOf("\"", passQuoteStart);
            String password = data.substring(passQuoteStart, passQuoteEnd);
            
            Settings::setWiFiCredentials(ssid, password);
            LOG(TAG, "WiFi credentials saved - SSID: %s", ssid.c_str());
            return true;
        }
        
        else {
            LOG(TAG, "ERROR: Unknown command: %s", command.c_str());
            return false;
        }

        return true;
    }
}
