#include <WiFi.h>
#include "WifiManager.h"
#include "Logging.h"
#include "Settings.h"

namespace WiFiManager {
    static const char *TAG = "WIFI";

    static String g_ssid;
    static String g_password;
    static bool wifiConnectedOnce = false;
    static unsigned long lastWiFiReconnectAttempt = 0;
    static unsigned long wifiReconnectInterval = 5000;
    static int failedReconnectAttempts = 0;
    static WifiStatusCallback statusCallback = nullptr;

    static void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
        LOG(TAG, "WiFi connected, IP: %s", WiFi.localIP().toString().c_str());
        if (statusCallback) statusCallback(true, WiFi.localIP().toString());
    }

    static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
        LOG(TAG, "Disconnected from WiFi. Reason: %d", info.wifi_sta_disconnected.reason);
        if (statusCallback) statusCallback(false, "Disconnected");
    }

    void init(WifiStatusCallback callback) {
        statusCallback = callback;
        WiFi.disconnect(true);
        delay(100);
        WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
        WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

        if (Settings::getWiFiCredentials(g_ssid, g_password)) {
            LOG(TAG, "Stored credentials found, connecting...");
            WiFi.begin(g_ssid.c_str(), g_password.c_str());
            wifiConnectedOnce = true;
        }
    }

    void connect(const String &ssid, const String &password) {
        LOG(TAG, "Connecting to %s...", ssid.c_str());
        g_ssid = ssid;
        g_password = password;

        WiFi.disconnect(true);
        delay(500);
        WiFi.begin(ssid.c_str(), password.c_str());
        wifiConnectedOnce = true;
        failedReconnectAttempts = 0;
        wifiReconnectInterval = 5000;
    }

    void handleReconnect() {
        if (wifiConnectedOnce && WiFi.status() != WL_CONNECTED) {
            unsigned long currentMillis = millis();
            if (currentMillis - lastWiFiReconnectAttempt >= wifiReconnectInterval) {
                lastWiFiReconnectAttempt = currentMillis;

                LOG(TAG, "Attempting to reconnect...");
                WiFi.begin(g_ssid.c_str(), g_password.c_str());

                failedReconnectAttempts++;
                if (wifiReconnectInterval < 60000) {
                    wifiReconnectInterval *= 2;
                    if (wifiReconnectInterval > 60000) wifiReconnectInterval = 60000;
                }
            }
        } else if (WiFi.status() == WL_CONNECTED) {
            if (failedReconnectAttempts > 0) {
                LOG(TAG, "WiFi Reconnected!");
                failedReconnectAttempts = 0;
                wifiReconnectInterval = 5000;
            }
        }
    }

    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
}
