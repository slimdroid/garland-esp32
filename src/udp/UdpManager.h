#pragma once

/**
 * @brief Default UDP port for discovery and handshake
 */
#define LOCAL_UDP_PORT 4210

/**
 * @brief Discovery response message
 */
#define HANDSHAKE_MESSAGE "LUMIX_DISCOVERY"

/**
 * @brief Callback type for receiving UDP messages
 * @param message Received message content
 * @return true if message was processed successfully
 */
typedef bool (*UdpMessageCallback)(const String &message);

/**
 * @brief Management of UDP communication for device discovery
 */
namespace UdpManager {
    /**
     * @brief Initializes and starts the UDP listener
     * @param port UDP port to listen on
     */
    void init(uint16_t port = LOCAL_UDP_PORT);

    /**
     * @brief Sets the callback function for incoming UDP messages
     * @param callback Callback function
     */
    void setMessageListener(UdpMessageCallback callback);

    /**
     * @brief Stops the UDP listener
     */
    void stop();

    /**
     * @brief Periodic handler to process incoming UDP packets
     */
    void handle();
}
