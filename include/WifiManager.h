#pragma once

typedef void (*WifiStatusCallback)(bool connected, const String &message);

namespace WiFiManager {
    void init(WifiStatusCallback callback);

    void connect(const String &ssid, const String &password);

    void handleReconnect();

    bool isConnected();
}
