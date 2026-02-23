# Data Exchange Protocol

## Description

The protocol is designed to control an ESP32 device with an RGB LED strip through external interfaces (WebSocket, UDP, Bluetooth).

Data format: JSON structure with commands and parameters.

## Transport Protocols

The protocol works over the following transports:
- **WebSocket** - for persistent connection
- **UDP** - for fast commands without connection establishment
- **Bluetooth** - for wireless control without WiFi

## Command Format

All commands are transmitted in JSON format:
```json
{"cmd":"<command_name>","param1":"value1","param2":"value2"}
```

## Supported Commands

### 1. Set LED Mode (set_mode)

Sets the operating mode (effect) of the RGB LED strip.

**Format:**
```json
{"cmd":"set_mode","mode":<0-13>}
```

**Parameters:**
- `mode` (integer, required) - mode number:
  - `0` - RAINBOW (rainbow)
  - `1` - CYLON (cylon scanner)
  - `2` - SPARKLE (sparkle)
  - `3` - FIRE (fire)
  - `4` - CONFETTI (confetti)
  - `5` - SINELON (sinelon)
  - `6` - JUGGLE (juggle)
  - `7` - BPM (bpm)
  - `8` - SNOW (snow)
  - `9` - COMET (comet)
  - `10` - RAINBOW_GLITTER (rainbow with glitter)
  - `11` - COLOR_WAVES (color waves)
  - `12` - THEATER_CHASE (theater chase)
  - `13` - SOLID_GLOW (solid glow)

**Examples:**
```json
{"cmd":"set_mode","mode":0}
{"cmd":"set_mode","mode":3}
{"cmd":"set_mode","mode":13}
```

**Result:**
- LED effect changes immediately
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
- Response format (in logs): `Status - Mode: <0-13>, Power: <ON|OFF>`

---

### 4. WiFi Configuration (set_wifi)

Sets credentials for connecting to a WiFi network.

**Format:**
```json
{"cmd":"set_wifi","ssid":"<network_name>","pass":"<password>"}
```

**Parameters:**
- `ssid` (string, required) - WiFi network name
- `pass` (string, optional) - WiFi network password (default: empty string)

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

1. **JSON parse error:**
   ```
   JSON parse error: <description>
   ```

2. **Missing 'cmd' field:**
   ```
   ERROR: Missing 'cmd' field
   ```

3. **Invalid parameter value:**
   ```
   Invalid mode value: <value>
   Invalid state value: <value>
   Invalid brightness value: <value>
   Invalid LED count: <value>
   ```

4. **Missing required parameter:**
   ```
   Missing or empty SSID
   ```

5. **Unknown command:**
   ```
   ERROR: Unknown command: <command_name>
   ```

---

## Usage Examples

### Example 1: Turn on rainbow effect
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":0}
```

### Example 2: Switch to fire effect
```json
{"cmd":"set_mode","mode":3}
```

### Example 3: Turn off the system
```json
{"cmd":"set_power","state":0}
```

### Example 4: Configure WiFi and enable snow effect
```json
{"cmd":"set_wifi","ssid":"MyHomeNetwork","pass":"password123"}
{"cmd":"set_mode","mode":8}
```

### Example 5: Check status
```json
{"cmd":"get_status"}
```

### Example 6: Set brightness to 50%
```json
{"cmd":"set_brightness","value":128}
```

### Example 7: Configure 60-LED strip
```json
{"cmd":"set_led_count","value":60}
```

---

## Protocol Testing

### Via WebSocket (JavaScript)
```javascript
const ws = new WebSocket('ws://<device_ip>:<port>');

ws.onopen = () => {
    // Set rainbow effect
    ws.send('{"cmd":"set_mode","mode":0}');
    
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

# Set fire effect
sock.sendto(b'{"cmd":"set_mode","mode":3}', (device_ip, device_port))
```

### Via UDP (netcat)
```bash
# Set RAINBOW mode
echo '{"cmd":"set_mode","mode":0}' | nc -u <device_ip> <port>

# Turn off system
echo '{"cmd":"set_power","state":0}' | nc -u <device_ip> <port>
```

---

## Settings Persistence

All commands that change device state automatically save settings to non-volatile memory (NVS):

- `set_mode` - saves current LED effect mode
- `set_power` - saves system power state
- `set_brightness` - saves brightness level
- `set_led_count` - saves number of LEDs
- `set_wifi` - saves WiFi credentials

When the device reboots, all settings are restored automatically.

---

## Technical Details

- **Encoding:** UTF-8
- **Maximum command length:** limited by String buffer size in Arduino (typically ~1KB)
- **Parsing:** ArduinoJson library
- **Logging:** all commands and errors are logged through the ESP logging module
- **Thread safety:** commands are processed in the main loop()

---

## Protocol Extension

To add a new command:

1. Add handling to the `DataParser::parse()` function in `src/parser/DataParser.cpp`
2. Use the existing command parsing pattern
3. Add logging for debugging
4. Update this documentation

Example of adding a new command:
```cpp
if (strcmp(cmd, "new_command") == 0) {
    int param = doc["param"] | -1;
    // ... validate and execute ...
    ESP_LOGI(TAG, "New command executed");
    return true;
}
```
