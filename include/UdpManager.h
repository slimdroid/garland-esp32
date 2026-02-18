#pragma once

#define LOCAL_UDP_PORT 4210
#define HANDSHAKE_MESSAGE "HELLO ESP"

typedef bool (*UdpMessageCallback)(const String &message);

namespace UdpManager {
    void init(uint16_t port = LOCAL_UDP_PORT);

    void setMessageListener(UdpMessageCallback callback);

    void stop();

    void handle();
}
