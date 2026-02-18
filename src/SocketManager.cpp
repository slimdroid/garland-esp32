#include <WiFi.h>
#include "SocketManager.h"
#include "Logging.h"

namespace SocketManager {
    static const char *TAG = "TCP_SOCKET";

    static WiFiServer *server = nullptr;
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

        WiFiClient client = server->available();
        if (client) {
            LOG(TAG, "TCP Client connected");
            while (client.connected()) {
                if (client.available()) {
                    String data = client.readStringUntil('\n');
                    data.trim();
                    LOG(TAG, "TCP Received: %s", data.c_str());
                    if (messageCallback != nullptr) {
                        if (messageCallback(data) == true) {
                            client.print("SUCCESS");
                        } else {
                            client.println("FAILURE");
                        }
                    }
                }

                if (millis() - lastHeartbeatMillis > HEARTBEAT_INTERVAL) {
                    lastHeartbeatMillis = millis();
                    client.println(lastHeartbeatMillis);
                    LOG(TAG, "TCP Heartbeat sent: %lu", lastHeartbeatMillis);
                }
                yield();
            }
            client.stop();
            LOG(TAG, "TCP Client disconnected");
        }
    }
}
