#pragma once

/**
 * @brief Default port for TCP socket communication
 */
#define TCP_SOCKET_PORT 5920

/**
 * @brief Callback type for receiving messages via TCP socket
 * @param message Received message string
 * @return true if message was processed successfully
 */
typedef bool (*SocketMessageCallback)(const String &message);

/**
 * @brief Management of TCP socket server for command processing
 */
namespace SocketManager {
    /**
     * @brief Initializes and starts the TCP server
     * @param port TCP port to listen on
     */
    void init(uint16_t port = TCP_SOCKET_PORT);

    /**
     * @brief Sets the callback function for incoming messages
     * @param callback Callback function
     */
    void setMessageListener(SocketMessageCallback callback);

    /**
     * @brief Stops the TCP server
     */
    void stop();

    /**
     * @brief Periodic handler to process new connections and incoming data
     */
    void handle();
}
