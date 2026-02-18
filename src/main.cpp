#include <Arduino.h>
#include "StringLED.h"
#include "BuiltInLed.h"
#include "Button.h"
#include "Settings.h"
#include "Bluetooth.h"
#include "Logging.h"
#include "WifiManager.h"
#include "UdpManager.h"
#include "SocketManager.h"
#include "DataParser.h"

#define BUTTON_PIN  5

static const char *TAG = "MAIN";

Button button(BUTTON_PIN);
BuiltInLed builtInLed;
StringLED::LightMode currentMode = StringLED::FADE;
bool isSystemOff = false;
unsigned long timerMillis = 0;

// Export pointers for DataParser
namespace DataParser {
    StringLED::LightMode *currentMode = &::currentMode;
    bool *isSystemOff = &::isSystemOff;
}

bool onCommandMessageReceived(const String &message) {
    return DataParser::parse(message);
}

void onWifiStatusChanged(bool connected, const String &message) {
    if (connected) {
        SocketManager::init();
        SocketManager::setMessageListener(onCommandMessageReceived);
        UdpManager::init();
        UdpManager::setMessageListener(onCommandMessageReceived);
        // Send info back to BLE
        Bluetooth::sendWiFiConnectInfo(true, message);
    } else {
        // Send info back to BLE
        Bluetooth::sendWiFiConnectInfo(false, message);
        SocketManager::setMessageListener(nullptr);
        SocketManager::stop();
        UdpManager::setMessageListener(nullptr);
        UdpManager::stop();
    }
}

void onBleDataReceived(String value) {
    LOG(TAG, "Received message: %s", value.c_str());

    int colonIndex = value.indexOf(':');
    if (colonIndex != -1) {
        String ssid = value.substring(0, colonIndex);
        String password = value.substring(colonIndex + 1);
        LOG(TAG, "Extracted SSID: %s", ssid.c_str());

        WiFiManager::connect(ssid, password);
        Settings::setWiFiCredentials(ssid, password);
    } else {
        LOG(TAG, "Invalid format received. Expected 'ssid:password'");
    }
}

void onBleStateChanged(BT_ConnectionState state) {
    switch (state) {
        case BT_ENABLED:
            builtInLed.switchOn(true, 1000);
            break;
        case BT_CONNECTED:
            builtInLed.switchOn(true, 200);
            break;
        case BT_DISCONNECTED:
            builtInLed.switchOn(true, 1000);
            break;
        case BT_DISABLED:
            builtInLed.switchOff();
            break;
    }
}

void setup() {
    Serial.begin(115200);

    // LED initializing
    StringLED::init();

    // Restore settings
    int savedMode;
    Settings::loadLightSettings(savedMode, isSystemOff);
    currentMode = static_cast<StringLED::LightMode>(savedMode);
    LOG(TAG, "Loaded mode: %d", currentMode);
    LOG(TAG, "System state: %s", isSystemOff ? "OFF" : "ON");

    WiFiManager::init(onWifiStatusChanged);

    Bluetooth::init(onBleDataReceived, onBleStateChanged);
}

static void handleTimer() {
    if (millis() - timerMillis > 1000) {
        timerMillis = millis();
        Bluetooth::sendWorkedTime(timerMillis / 1000);
    }
}

void loop() {
    builtInLed.handle();

    handleTimer();

    const ButtonAction action = button.handle();
    switch (action) {
        case SHORT_PRESS:
            if (!isSystemOff) {
                currentMode = static_cast<StringLED::LightMode>((currentMode + 1) % StringLED::NUM_MODES);
                Settings::saveLightMode(currentMode);
                LOG(TAG, "Mode changed to: %d", currentMode);
            }
            break;
        case MEDIUM_PRESS:
            isSystemOff = !isSystemOff;
            Settings::saveSystemState(isSystemOff);
            if (isSystemOff) {
                LOG(TAG, "System OFF");
                Bluetooth::disable();
            } else {
                LOG(TAG, "System ON, Mode: %d", currentMode);
            }
            break;
        case LONG_PRESS:
            if (!isSystemOff) {
                Bluetooth::enable();
            } else {
                isSystemOff = false;
                Settings::saveSystemState(isSystemOff);
                LOG(TAG, "System ON, Mode: %d", currentMode);
            }
            break;
        case NO_ACTION:
        default:
            break;
    }

    // Mode execution
    StringLED::handleLEDs(currentMode, isSystemOff);

    Settings::handleSettingsSync();

    WiFiManager::handleReconnect();

    if (WiFiManager::isConnected()) {
        UdpManager::handle();
        SocketManager::handle();
    }
}
