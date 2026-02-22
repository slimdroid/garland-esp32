# ESP32 Garland LED Strip Controller

## Project Description

This is a firmware project for ESP32 microcontroller that provides remote control of addressable LED strips (WS2812B or similar) via multiple communication interfaces. The device supports WiFi connectivity, Bluetooth Low Energy (BLE), WebSocket, and UDP protocols for flexible control options.

### Key Features

- **LED Strip Control:**
  - Full control over addressable LED strips (WS2812B or compatible) via FastLED library
  - Configurable number of LEDs (1–256, default: 30)
  - Adjustable brightness (0–255, default: 51)
  - 14 built-in lighting effects

- **Lighting Effects:**
  - RAINBOW - smooth rainbow cycling
  - CYLON - bouncing scanner effect
  - SPARKLE - random sparkling
  - FIRE - fire simulation
  - CONFETTI - random colorful flashes
  - SINELON - sine wave scanning dot
  - JUGGLE - multiple dots juggling
  - BPM - beat-synced palette blending
  - SNOW - falling snow effect
  - COMET - comet trail effect
  - RAINBOW_GLITTER - rainbow with glitter
  - COLOR_WAVES - flowing color waves
  - THEATER_CHASE - theater-style chasing lights
  - SOLID_GLOW - steady solid color glow

- **Multiple Control Interfaces:**
  - Bluetooth Low Energy (BLE) for initial setup and mobile control
  - WiFi connectivity with automatic reconnection
  - WebSocket server for real-time bidirectional communication
  - UDP server for fast command delivery

- **Physical Controls:**
  - Button support for local control
  - Built-in LED status indicator for BLE state
  - Short press — cycle through LED effects
  - Medium press — toggle system power
  - Long press — enable Bluetooth pairing

- **Persistent Settings:**
  - All settings stored in non-volatile memory (NVS)
  - WiFi credentials persistence
  - LED effect, brightness, LED count, and power state restoration after reboot

- **Command Protocol:**
  - JSON command format
  - Support for mode switching, brightness/LED count control, power control, status queries, and WiFi configuration
  - See [PROTOCOL.md](PROTOCOL.md) for detailed protocol documentation

### Supported Boards

| Board | Environment |
|---|---|
| ESP32-DOIT-DEVKIT-V1 | `esp32doit-devkit-v1` |
| M5Stack NanoC6 | `m5stack-nanoc6` |
| M5Stack AtomS3 | `m5stack-atoms3` |

### Hardware Requirements

- One of the supported ESP32 development boards (see above)
- Addressable LED strip (WS2812B or similar)
- Push button
- Power supply

### Software Stack

- **Platform:** PlatformIO
- **Framework:** Arduino for ESP32
- **LED Library:** FastLED @ ^3.10.3
- **Platform:** pioarduino espressif32 (develop)

### Getting Started

1. **Select the target board and build/upload:**
   ```bash
   pio run -e esp32doit-devkit-v1 -t upload
   # or
   pio run -e m5stack-nanoc6 -t upload
   # or
   pio run -e m5stack-atoms3 -t upload
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

- [FastLED Library](https://github.com/FastLED/FastLED)
- [ESP32 Parallel Execution](https://randomnerdtutorials.com/projects-esp32/)
- [ESP32 Dual Core Arduino IDE](https://randomnerdtutorials.com/esp32-dual-core-arduino-ide)
- [ESP32 TCP/IP Socket Server Example](https://github.com/phfbertoleti/esp32_tcp_ip_socket_server_example/blob/main/esp32_tcp_server_socket_example/main/socket_tcp_server/socket_tcp_server.c)

### License

This project is provided as-is for educational and development purposes.
