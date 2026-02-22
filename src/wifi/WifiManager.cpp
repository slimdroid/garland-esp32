#include <WiFi.h>
#include "WifiManager.h"
#include "../settings/Settings.h"

namespace WiFiManager {
    static const char *TAG = "WIFI";

    static String g_ssid;
    static String g_password;
    static bool hasCredentials = false;
    static unsigned long lastWiFiReconnectAttempt = 0;
    static unsigned long wifiReconnectInterval = 5000;
    static int failedReconnectAttempts = 0;
    static WifiStatusCallback statusCallback = nullptr;

    static void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
        ESP_LOGI(TAG, "WiFi connected, IP: %s", WiFi.localIP().toString().c_str());
        if (statusCallback) statusCallback(true, WiFi.localIP().toString());
    }

    static void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
        ESP_LOGW(TAG, "Disconnected from WiFi. Reason: %d", info.wifi_sta_disconnected.reason);
        if (statusCallback) statusCallback(false, "Disconnected");
    }

    void init(WifiStatusCallback callback) {
        statusCallback = callback;
        WiFi.disconnect(true);
        delay(100);
        WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
        WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

        if (Settings::getWiFiCredentials(g_ssid, g_password)) {
            ESP_LOGI(TAG, "Stored credentials found, connecting...");
            WiFi.begin(g_ssid.c_str(), g_password.c_str());
            hasCredentials = true;
        }
    }

    void connect(const String &ssid, const String &password) {
        ESP_LOGI(TAG, "Connecting to %s...", ssid.c_str());
        g_ssid = ssid;
        g_password = password;

        WiFi.disconnect(true);
        delay(500);
        WiFi.begin(ssid.c_str(), password.c_str());
        hasCredentials = true;
        failedReconnectAttempts = 0;
        wifiReconnectInterval = 5000;
    }

    void handleReconnect() {
        if (hasCredentials && WiFi.status() != WL_CONNECTED) {
            unsigned long currentMillis = millis();
            if (currentMillis - lastWiFiReconnectAttempt >= wifiReconnectInterval) {
                lastWiFiReconnectAttempt = currentMillis;

                ESP_LOGI(TAG, "Attempting to reconnect...");
                WiFi.begin(g_ssid.c_str(), g_password.c_str());

                failedReconnectAttempts++;
                if (wifiReconnectInterval < 60000) {
                    wifiReconnectInterval *= 2;
                    if (wifiReconnectInterval > 60000) wifiReconnectInterval = 60000;
                }
            }
        } else if (WiFi.status() == WL_CONNECTED) {
            if (failedReconnectAttempts > 0) {
                ESP_LOGI(TAG, "WiFi Reconnected!");
                failedReconnectAttempts = 0;
                wifiReconnectInterval = 5000;
            }
        }
    }

    bool isConnected() {
        return WiFi.status() == WL_CONNECTED;
    }
}
