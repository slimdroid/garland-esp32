# Data Exchange Protocol

## Description

The protocol is designed to control an ESP32 device with an RGB LED through external interfaces (WebSocket, UDP).

Data format: JSON-like structure with commands and parameters.

## Transport Protocols

The protocol works over the following transports:
- **WebSocket** - for persistent connection
- **UDP** - for fast commands without connection establishment

## Command Format

All commands are transmitted in JSON format:
```json
{"cmd":"<command_name>","param1":"value1","param2":"value2"}
```

## Supported Commands

### 1. Set LED Mode (set_mode)

Sets the operating mode of the RGB LED.

**Format:**
```json
{"cmd":"set_mode","mode":<0-4>}
```

**Parameters:**
- `mode` (integer, required) - mode number:
  - `0` - FADE (smooth color transitions)
  - `1` - RED (red)
  - `2` - GREEN (green)
  - `3` - BLUE (blue)
  - `4` - WHITE (white)

**Examples:**
```json
{"cmd":"set_mode","mode":0}
{"cmd":"set_mode","mode":2}
{"cmd":"set_mode","mode":4}
```

**Result:**
- LED mode changes immediately
- New mode is saved to non-volatile memory
- Returns `true` on success, `false` on error

---

### 2. System Power Control (set_power)

Turns the LED control system on or off.

**Format:**
```json
{"cmd":"set_power","state":<0|1>}
```

**Parameters:**
- `state` (integer, required):
  - `0` - turn system off (OFF)
  - `1` - turn system on (ON)

**Examples:**
```json
{"cmd":"set_power","state":0}
{"cmd":"set_power","state":1}
```

**Result:**
- System state changes immediately
- New state is saved to non-volatile memory
- When turned off, LED goes dark regardless of mode
- Returns `true` on success, `false` on error

---

### 3. Status Query (get_status)

Gets the current device state.

**Format:**
```json
{"cmd":"get_status"}
```

**Parameters:**
No parameters.

**Example:**
```json
{"cmd":"get_status"}
```

**Result:**
- Information about current mode and power state is output to log
- Returns `true` on success
- Response format (in logs): `Status - Mode: <0-4>, Power: <ON|OFF>`

---

### 4. WiFi Configuration (set_wifi)

Sets credentials for connecting to a WiFi network.

**Format:**
```json
{"cmd":"set_wifi","ssid":"<network_name>","pass":"<password>"}
```

**Parameters:**
- `ssid` (string, required) - WiFi network name
- `pass` (string, required) - WiFi network password

**Examples:**
```json
{"cmd":"set_wifi","ssid":"MyNetwork","pass":"MyPassword123"}
{"cmd":"set_wifi","ssid":"HomeWiFi","pass":"secure_pass"}
```

**Result:**
- Credentials are saved to non-volatile memory
- Device will attempt to connect to the new network on next reconnection
- Returns `true` on success, `false` on error

---

### 5. Set Brightness (set_brightness)

Sets the brightness of the LED strip.

**Format:**
```json
{"cmd":"set_brightness","value":<0-255>}
```

**Parameters:**
- `value` (integer, required) - Brightness level from 0 (off) to 255 (maximum). Default: 51 (20% of maximum).

**Examples:**
```json
{"cmd":"set_brightness","value":51}
{"cmd":"set_brightness","value":128}
{"cmd":"set_brightness","value":255}
```

**Result:**
- Brightness is applied immediately and saved to non-volatile memory
- Returns `true` on success, `false` on error

---

### 6. Set LED Count (set_led_count)

Sets the number of LEDs in the strip.

**Format:**
```json
{"cmd":"set_led_count","value":<1-256>}
```

**Parameters:**
- `value` (integer, required) - Number of LEDs from 1 to 256. Default: 30.

**Examples:**
```json
{"cmd":"set_led_count","value":30}
{"cmd":"set_led_count","value":60}
{"cmd":"set_led_count","value":144}
```

**Result:**
- New LED count is applied immediately and saved to non-volatile memory
- Returns `true` on success, `false` on error

---

## Error Handling

When an error occurs, the command returns `false` and outputs an error message to the log.

**Error Types:**

1. **Missing 'cmd' field:**
   ```
   ERROR: No 'cmd' field found
   ```

2. **Invalid command format:**
   ```
   ERROR: Invalid command format
   ```

3. **Missing required parameter:**
   ```
   ERROR: No 'mode' parameter
   ERROR: No 'state' parameter
   ERROR: Missing 'ssid' or 'pass' parameter
   ```

4. **Invalid parameter value:**
   ```
   ERROR: Invalid mode value: <value>
   ```

5. **Unknown command:**
   ```
   ERROR: Unknown command: <command_name>
   ```

6. **Internal error:**
   ```
   ERROR: currentMode pointer is null
   ERROR: isSystemOff pointer is null
   ERROR: State pointers are null
   ```

---

## Usage Examples

### Example 1: Turn on red color
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":1}
```

### Example 2: Switch to smooth color transition mode
```json
{"cmd":"set_mode","mode":0}
```

### Example 3: Turn off the system
```json
{"cmd":"set_power","state":0}
```

### Example 4: Configure WiFi and turn on green color
```json
{"cmd":"set_wifi","ssid":"MyHomeNetwork","pass":"password123"}
{"cmd":"set_mode","mode":2}
```

### Example 5: Check status
```json
{"cmd":"get_status"}
```

---

## Protocol Testing

### Via WebSocket (JavaScript)
```javascript
const ws = new WebSocket('ws://<device_ip>:<port>');

ws.onopen = () => {
    // Set blue color
    ws.send('{"cmd":"set_mode","mode":3}');
    
    // Request status
    ws.send('{"cmd":"get_status"}');
};
```

### Via UDP (Python)
```python
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
device_ip = '<device_ip>'
device_port = <port>

# Turn on system
sock.sendto(b'{"cmd":"set_power","state":1}', (device_ip, device_port))

# Set white color
sock.sendto(b'{"cmd":"set_mode","mode":4}', (device_ip, device_port))
```

### Via UDP (netcat)
```bash
# Set FADE mode
echo '{"cmd":"set_mode","mode":0}' | nc -u <device_ip> <port>

# Turn off system
echo '{"cmd":"set_power","state":0}' | nc -u <device_ip> <port>
```

---

## Settings Persistence

All commands that change device state automatically save settings to non-volatile memory (NVS):

- `set_mode` - saves current LED mode
- `set_power` - saves system power state
- `set_wifi` - saves WiFi credentials

When the device reboots, all settings are restored automatically.

---

## Technical Details

- **Encoding:** UTF-8
- **Maximum command length:** limited by String buffer size in Arduino (typically ~1KB)
- **Parsing:** simple string-based parser without external libraries
- **Logging:** all commands and errors are logged through the Logging module
- **Thread safety:** commands are processed in the main loop()

---

## Protocol Extension

To add a new command:

1. Add handling to the `DataParser::parse()` function in `src/DataParser.cpp`
2. Use the existing command parsing pattern
3. Add logging for debugging
4. Update this documentation

Example of adding a new command:
```cpp
else if (command == "new_command") {
    // Extract parameters
    int paramStart = data.indexOf("\"param\"");
    // ... parse parameters ...
    
    // Execute action
    // ... your code ...
    
    ESP_LOGI(TAG, "New command executed");
    return true;
}
```
