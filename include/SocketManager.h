#pragma once

#define TCP_SOCKET_PORT 5920

typedef bool (*SocketMessageCallback)(const String &message);

namespace SocketManager {
    void init(uint16_t port = TCP_SOCKET_PORT);

    void setMessageListener(SocketMessageCallback callback);

    void stop();

    void handle();
}
