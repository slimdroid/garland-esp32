#include <WiFi.h>
#include "SocketManager.h"
#include "Logging.h"

namespace SocketManager {
    static const char *TAG = "TCP_SOCKET";

    static WiFiServer *server = nullptr;
    static WiFiClient currentClient;
    static unsigned long lastHeartbeatMillis = 0;
    static constexpr unsigned long HEARTBEAT_INTERVAL = 5000;
    static bool socketRunning = false;
    static SocketMessageCallback messageCallback = nullptr;

    void init(uint16_t port) {
        if (server) delete server;
        server = new WiFiServer(port);
        server->begin();
        socketRunning = true;
        LOG(TAG, "TCP Server started on port %d", port);
    }

    void setMessageListener(SocketMessageCallback callback) {
        messageCallback = callback;
    }

    void stop() {
        if (server) {
            server->stop();
        }
        socketRunning = false;
        LOG(TAG, "TCP Server stopped");
    }

    void handle() {
        if (!socketRunning || !server) return;

        if (!currentClient || !currentClient.connected()) {
            currentClient = server->available();
            if (currentClient) {
                LOG(TAG, "TCP Client connected");
            }
        }

        if (currentClient && currentClient.connected()) {
            if (currentClient.available()) {
                String data = currentClient.readStringUntil('\n');
                data.trim();
                LOG(TAG, "TCP Received: %s", data.c_str());
                if (messageCallback != nullptr) {
                    if (messageCallback(data) == true) {
                        currentClient.print("SUCCESS");
                    } else {
                        currentClient.println("FAILURE");
                    }
                }
            }

            if (millis() - lastHeartbeatMillis > HEARTBEAT_INTERVAL) {
                lastHeartbeatMillis = millis();
                currentClient.println(lastHeartbeatMillis);
                LOG(TAG, "TCP Heartbeat sent: %lu", lastHeartbeatMillis);
            }
        }
    }
}
