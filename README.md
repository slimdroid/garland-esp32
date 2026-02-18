# ESP32 RGB LED Controller

## Project Description

This is a firmware project for ESP32 microcontroller that provides remote control of RGB LED strips via multiple communication interfaces. The device supports WiFi connectivity, Bluetooth Low Energy (BLE), WebSocket, and UDP protocols for flexible control options.

### Key Features

- **Multiple Control Interfaces:**
  - Bluetooth Low Energy (BLE) for initial setup and mobile control
  - WiFi connectivity with automatic reconnection
  - WebSocket server for real-time bidirectional communication
  - UDP server for fast command delivery
  
- **LED Control Modes:**
  - FADE - smooth color transitions
  - RED - solid red color
  - GREEN - solid green color
  - BLUE - solid blue color
  - WHITE - solid white color

- **Physical Controls:**
  - Button support for local control
  - Built-in LED status indicator
  - Short press - cycle through LED modes
  - Medium press - toggle system power
  - Long press - enable Bluetooth pairing

- **Persistent Settings:**
  - All settings stored in non-volatile memory (NVS)
  - WiFi credentials persistence
  - LED mode and power state restoration after reboot

- **Command Protocol:**
  - JSON-like command format
  - Support for mode switching, power control, status queries, and WiFi configuration
  - See [PROTOCOL.md](PROTOCOL.md) for detailed protocol documentation

### Hardware Requirements

- ESP32 development board (ESP32-DOIT-DEVKIT-V1 or compatible)
- RGB LED strip (WS2812B or similar)
- Push button
- Power supply

### Software Stack

- **Platform:** PlatformIO
- **Framework:** Arduino for ESP32
- **Board:** ESP32-DOIT-DEVKIT-V1
- **Platform Version:** espressif32 @ 6.12.0

### Project Structure

```
├── include/          # Header files
├── src/              # Source files
│   ├── main.cpp      # Main application entry point
│   ├── Bluetooth.cpp # BLE functionality
│   ├── DataParser.cpp # Command parser
│   ├── StringLED.cpp # LED control
│   ├── Button.cpp    # Button handling
│   ├── Settings.cpp  # NVS settings management
│   ├── WifiManager.cpp # WiFi connection management
│   ├── SocketManager.cpp # WebSocket server
│   └── UdpManager.cpp # UDP server
├── PROTOCOL.md       # Communication protocol documentation
├── EXAMPLES.md       # Usage examples
└── platformio.ini    # PlatformIO configuration
```

### Getting Started

1. **Build and Upload:**
   ```bash
   pio run -t upload
   ```

2. **Monitor Serial Output:**
   ```bash
   pio device monitor
   ```

3. **Initial Setup:**
   - Long press the button to enable Bluetooth
   - Connect via BLE and send WiFi credentials in format: `ssid:password`
   - Device will connect to WiFi and start WebSocket/UDP servers

4. **Control the Device:**
   - See [EXAMPLES.md](EXAMPLES.md) for command examples
   - See [PROTOCOL.md](PROTOCOL.md) for protocol specification

### Documentation

- **[PROTOCOL.md](PROTOCOL.md)** - Complete communication protocol specification
- **[EXAMPLES.md](EXAMPLES.md)** - Usage examples for different platforms

### References

- [Data Parsing Example](https://www.youtube.com/watch?v=Ul03Vkg9A40)
- [ESP32 Parallel Execution](https://randomnerdtutorials.com/projects-esp32/)
- [ESP32 Dual Core Arduino IDE](https://randomnerdtutorials.com/esp32-dual-core-arduino-ide)
- [ESP32 TCP/IP Socket Server Example](https://github.com/phfbertoleti/esp32_tcp_ip_socket_server_example/blob/main/esp32_tcp_server_socket_example/main/socket_tcp_server/socket_tcp_server.c)

### License

This project is provided as-is for educational and development purposes.
