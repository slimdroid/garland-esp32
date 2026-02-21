#include <Arduino.h>
#include <esp32-hal-log.h>
#include "indicator/RgbIndicator.h"
#include "built_in_led/BuiltInLed.h"
#include "button/Button.h"
#include "settings/Settings.h"
#include "bluetooth/Bluetooth.h"
#include "wifi/WifiManager.h"
#include "udp/UdpManager.h"
#include "socket/SocketManager.h"
#include "parser/DataParser.h"
#include "board/BoardSelector.h"
#include <FastLED.h>

#undef ARDUHAL_LOG_FORMAT
#define ARDUHAL_LOG_FORMAT(letter, format) ARDUHAL_LOG_COLOR_ ## letter "[" #letter "]: " format ARDUHAL_LOG_RESET_COLOR "\r\n"

static const char *TAG = "MAIN";

Button button(Pins::BUTTON);
BuiltInLed builtInLed(Pins::LED);
RgbIndicator::LightMode currentMode = RgbIndicator::FADE;
bool isSystemOff = false;
unsigned long timerMillis = 0;

// Export pointers for DataParser
namespace DataParser {
    RgbIndicator::LightMode *currentMode = &::currentMode;
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
    ESP_LOGI(TAG, "Received message: %s", value.c_str());

    int colonIndex = value.indexOf(':');
    if (colonIndex != -1) {
        String ssid = value.substring(0, colonIndex);
        String password = value.substring(colonIndex + 1);
        ESP_LOGI(TAG, "Extracted SSID: %s", ssid.c_str());

        WiFiManager::connect(ssid, password);
        Settings::setWiFiCredentials(ssid, password);
    } else {
        ESP_LOGW(TAG, "Invalid format received. Expected 'ssid:password'");
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
    RgbIndicator::init(Pins::LED_R, Pins::LED_G, Pins::LED_B);

    // Restore settings
    int savedMode;
    Settings::loadLightSettings(savedMode, isSystemOff);
    currentMode = static_cast<RgbIndicator::LightMode>(savedMode);
    ESP_LOGI(TAG, "Loaded mode: %d", currentMode);
    ESP_LOGI(TAG, "System state: %s", isSystemOff ? "OFF" : "ON");

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
                currentMode = static_cast<RgbIndicator::LightMode>((currentMode + 1) % RgbIndicator::NUM_MODES);
                Settings::saveLightMode(currentMode);
                ESP_LOGI(TAG, "Mode changed to: %d", currentMode);
            }
            break;
        case MEDIUM_PRESS:
            isSystemOff = !isSystemOff;
            Settings::saveSystemState(isSystemOff);
            if (isSystemOff) {
                ESP_LOGI(TAG, "System OFF");
                Bluetooth::disable();
            } else {
                ESP_LOGI(TAG, "System ON, Mode: %d", currentMode);
            }
            break;
        case LONG_PRESS:
            if (!isSystemOff) {
                Bluetooth::enable();
            } else {
                isSystemOff = false;
                Settings::saveSystemState(isSystemOff);
                ESP_LOGI(TAG, "System ON, Mode: %d", currentMode);
            }
            break;
        case NO_ACTION:
        default:
            break;
    }

    // Mode execution
    RgbIndicator::handleLEDs(currentMode, isSystemOff);

    Settings::handleSettingsSync();

    WiFiManager::handleReconnect();

    if (WiFiManager::isConnected()) {
        UdpManager::handle();
        SocketManager::handle();
    }
}
