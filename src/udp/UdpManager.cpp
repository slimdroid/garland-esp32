#include <WiFiUdp.h>
#include "UdpManager.h"
#include <WiFi.h>
#include "../Logging.h"

#define COMMAND_MARK "Cmd"

namespace UdpManager {
    static const char *TAG = "UDP";

    static WiFiUDP udp;
    static bool udpRunning = false;
    static UdpMessageCallback messageCallback = nullptr;

    static String prepareResponse() {
        uint64_t chipId = ESP.getEfuseMac();
        uint32_t chipIdLower = (uint32_t) (chipId & 0xFFFFFFFF);

        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                 (uint8_t) (chipId >> 0),
                 (uint8_t) (chipId >> 8),
                 (uint8_t) (chipId >> 16),
                 (uint8_t) (chipId >> 24),
                 (uint8_t) (chipId >> 32),
                 (uint8_t) (chipId >> 40));

        String ip = WiFi.localIP().toString();
        String version = APP_VERSION;
        String type = DEVICE_TYPE;
        String deviceName = "Device-" + String(chipIdLower % 10000, 10);

        String response = "{";
        response += "\"ip\":\"" + ip + "\",";
        response += "\"type\":\"" + type + "\",";
        response += "\"name\":\"" + deviceName + "\",";
        response += "\"device_id\":\"" + String(macStr) + "\",";
        response += "\"app_version\":\"" + version + "\"";
        response += "}";
        return response;
    }

    void init(uint16_t port) {
        if (udp.begin(port)) {
            udpRunning = true;
            LOG(TAG, "UDP started on port %d", port);
        } else {
            LOG(TAG, "Failed to start UDP");
        }
    }

    void stop() {
        udp.stop();
        udpRunning = false;
        LOG(TAG, "UDP stopped");
    }

    void handle() {
        if (!udpRunning) return;

        int packetSize = udp.parsePacket();
        if (packetSize > 0) {
            char packetBuffer[256];
            int len = udp.read(packetBuffer, sizeof(packetBuffer) - 1);
            if (len > 0) {
                packetBuffer[len] = 0;
            }
            String message = String(packetBuffer);
            message.trim();
            String remoteIP = udp.remoteIP().toString();
            LOG(TAG, "UDP Received: %s from %s", message.c_str(), remoteIP.c_str());

            if (message == HANDSHAKE_MESSAGE) {
                udp.beginPacket(udp.remoteIP(), udp.remotePort());
                String response = prepareResponse();
                udp.print(response);
                udp.endPacket();
                LOG(TAG, "UDP Sent response to %s", remoteIP.c_str());
            } else if (message.startsWith(COMMAND_MARK)) {
                // Notify listener if registered
                if (messageCallback != nullptr) {
                    udp.beginPacket(udp.remoteIP(), udp.remotePort());
                    if (messageCallback(message) == true) {
                        udp.print("SUCCESS");
                    } else {
                        udp.print("FAILURE");
                    }
                    udp.endPacket();
                }
            }
        }
    }

    void setMessageListener(UdpMessageCallback callback) {
        messageCallback = callback;
        LOG(TAG, "Message listener %s", callback != nullptr ? "registered" : "unregistered");
    }
}
