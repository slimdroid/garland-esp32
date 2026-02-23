# LED Control Command Examples

## Turning LED On and Off

### Turn on LED
```json
{"cmd":"set_power","state":1}
```

### Turn off LED
```json
{"cmd":"set_power","state":0}
```

---

## Switching LED Modes

### Mode 0: RAINBOW (rainbow)
```json
{"cmd":"set_mode","mode":0}
```

### Mode 1: CYLON (cylon scanner)
```json
{"cmd":"set_mode","mode":1}
```

### Mode 2: SPARKLE (sparkle)
```json
{"cmd":"set_mode","mode":2}
```

### Mode 3: FIRE (fire)
```json
{"cmd":"set_mode","mode":3}
```

### Mode 4: CONFETTI (confetti)
```json
{"cmd":"set_mode","mode":4}
```

### Mode 5: SINELON (sinelon)
```json
{"cmd":"set_mode","mode":5}
```

### Mode 6: JUGGLE (juggle)
```json
{"cmd":"set_mode","mode":6}
```

### Mode 7: BPM (bpm)
```json
{"cmd":"set_mode","mode":7}
```

### Mode 8: SNOW (snow)
```json
{"cmd":"set_mode","mode":8}
```

### Mode 9: COMET (comet)
```json
{"cmd":"set_mode","mode":9}
```

### Mode 10: RAINBOW_GLITTER (rainbow with glitter)
```json
{"cmd":"set_mode","mode":10}
```

### Mode 11: COLOR_WAVES (color waves)
```json
{"cmd":"set_mode","mode":11}
```

### Mode 12: THEATER_CHASE (theater chase)
```json
{"cmd":"set_mode","mode":12}
```

### Mode 13: SOLID_GLOW (solid glow)
```json
{"cmd":"set_mode","mode":13}
```

---

## Typical Usage Scenarios

### Scenario 1: Turn on LED in rainbow mode
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":0}
```

### Scenario 2: Switch to fire effect
```json
{"cmd":"set_mode","mode":3}
```

### Scenario 3: Enable snow effect
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":8}
```

### Scenario 4: Turn off LED
```json
{"cmd":"set_power","state":0}
```

### Scenario 5: Solid glow at full brightness
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":13}
{"cmd":"set_brightness","value":255}
```

### Scenario 6: Configure strip and set effect
```json
{"cmd":"set_led_count","value":60}
{"cmd":"set_brightness","value":128}
{"cmd":"set_mode","mode":0}
```

---

## Setting Brightness

### Set brightness to 20% (default)
```json
{"cmd":"set_brightness","value":51}
```

### Set brightness to 50%
```json
{"cmd":"set_brightness","value":128}
```

### Set maximum brightness
```json
{"cmd":"set_brightness","value":255}
```

---

## Configuring LED Count

### Set 30 LEDs (default)
```json
{"cmd":"set_led_count","value":30}
```

### Set 60 LEDs
```json
{"cmd":"set_led_count","value":60}
```

### Set 144 LEDs
```json
{"cmd":"set_led_count","value":144}
```

---

## Sending Commands via Different Interfaces

### Via WebSocket (JavaScript in browser)

```javascript
// Connect to device
const ws = new WebSocket('ws://192.168.1.100:81');

ws.onopen = () => {
    console.log('Connected to device');
    
    // Turn on LED
    ws.send('{"cmd":"set_power","state":1}');
    
    // Set rainbow effect
    ws.send('{"cmd":"set_mode","mode":0}');
};

ws.onmessage = (event) => {
    console.log('Response:', event.data);
};

// Control functions
function turnOn() {
    ws.send('{"cmd":"set_power","state":1}');
}

function turnOff() {
    ws.send('{"cmd":"set_power","state":0}');
}

function setMode(mode) {
    ws.send(`{"cmd":"set_mode","mode":${mode}}`);
}

function setBrightness(value) {
    ws.send(`{"cmd":"set_brightness","value":${value}}`);
}

function setLedCount(value) {
    ws.send(`{"cmd":"set_led_count","value":${value}}`);
}

// Usage examples:
// turnOn();              // Turn on
// setMode(0);            // Rainbow
// setMode(1);            // Cylon
// setMode(2);            // Sparkle
// setMode(3);            // Fire
// setMode(4);            // Confetti
// setMode(5);            // Sinelon
// setMode(6);            // Juggle
// setMode(7);            // BPM
// setMode(8);            // Snow
// setMode(9);            // Comet
// setMode(10);           // Rainbow with glitter
// setMode(11);           // Color waves
// setMode(12);           // Theater chase
// setMode(13);           // Solid glow
// setBrightness(128);    // 50% brightness
// setLedCount(60);       // 60 LEDs
// turnOff();             // Turn off
```

### Via UDP (Python)

```python
import socket
import time

# Connection settings
DEVICE_IP = '192.168.1.100'
DEVICE_PORT = 3333

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_command(cmd):
    sock.sendto(cmd.encode(), (DEVICE_IP, DEVICE_PORT))
    print(f'Sent: {cmd}')

# Configure strip
send_command('{"cmd":"set_led_count","value":60}')
time.sleep(0.1)

# Set brightness to 50%
send_command('{"cmd":"set_brightness","value":128}')
time.sleep(0.1)

# Turn on LED
send_command('{"cmd":"set_power","state":1}')
time.sleep(0.1)

# Cycle through effects
send_command('{"cmd":"set_mode","mode":0}')   # Rainbow
time.sleep(2)

send_command('{"cmd":"set_mode","mode":3}')   # Fire
time.sleep(2)

send_command('{"cmd":"set_mode","mode":8}')   # Snow
time.sleep(2)

send_command('{"cmd":"set_mode","mode":9}')   # Comet
time.sleep(2)

send_command('{"cmd":"set_mode","mode":13}')  # Solid glow
time.sleep(2)

# Turn off LED
send_command('{"cmd":"set_power","state":0}')

sock.close()
```

### Via UDP (Linux/macOS command line)

```bash
# Your device IP address
DEVICE_IP="192.168.1.100"
PORT="3333"

# Turn on LED
echo '{"cmd":"set_power","state":1}' | nc -u $DEVICE_IP $PORT

# Set rainbow effect
echo '{"cmd":"set_mode","mode":0}' | nc -u $DEVICE_IP $PORT

# Set fire effect
echo '{"cmd":"set_mode","mode":3}' | nc -u $DEVICE_IP $PORT

# Set snow effect
echo '{"cmd":"set_mode","mode":8}' | nc -u $DEVICE_IP $PORT

# Set comet effect
echo '{"cmd":"set_mode","mode":9}' | nc -u $DEVICE_IP $PORT

# Set solid glow
echo '{"cmd":"set_mode","mode":13}' | nc -u $DEVICE_IP $PORT

# Set brightness to 50%
echo '{"cmd":"set_brightness","value":128}' | nc -u $DEVICE_IP $PORT

# Set 60 LEDs
echo '{"cmd":"set_led_count","value":60}' | nc -u $DEVICE_IP $PORT

# Turn off LED
echo '{"cmd":"set_power","state":0}' | nc -u $DEVICE_IP $PORT
```

### Via UDP (Windows PowerShell)

```powershell
$DEVICE_IP = "192.168.1.100"
$PORT = 3333

$udpClient = New-Object System.Net.Sockets.UdpClient
$endpoint = New-Object System.Net.IPEndPoint([System.Net.IPAddress]::Parse($DEVICE_IP), $PORT)

function Send-Command($cmd) {
    $bytes = [System.Text.Encoding]::UTF8.GetBytes($cmd)
    $udpClient.Send($bytes, $bytes.Length, $endpoint)
    Write-Host "Sent: $cmd"
}

# Configure strip
Send-Command '{"cmd":"set_led_count","value":60}'
Start-Sleep -Milliseconds 100

# Set brightness
Send-Command '{"cmd":"set_brightness","value":128}'
Start-Sleep -Milliseconds 100

# Turn on LED
Send-Command '{"cmd":"set_power","state":1}'
Start-Sleep -Milliseconds 100

# Set rainbow effect
Send-Command '{"cmd":"set_mode","mode":0}'
Start-Sleep -Seconds 2

# Switch to fire
Send-Command '{"cmd":"set_mode","mode":3}'
Start-Sleep -Seconds 2

# Switch to snow
Send-Command '{"cmd":"set_mode","mode":8}'
Start-Sleep -Seconds 2

# Turn off
Send-Command '{"cmd":"set_power","state":0}'

$udpClient.Close()
```

---

## Quick Reference

| Action | Command |
|--------|---------|
| Turn on | `{"cmd":"set_power","state":1}` |
| Turn off | `{"cmd":"set_power","state":0}` |
| Get status | `{"cmd":"get_status"}` |
| RAINBOW | `{"cmd":"set_mode","mode":0}` |
| CYLON | `{"cmd":"set_mode","mode":1}` |
| SPARKLE | `{"cmd":"set_mode","mode":2}` |
| FIRE | `{"cmd":"set_mode","mode":3}` |
| CONFETTI | `{"cmd":"set_mode","mode":4}` |
| SINELON | `{"cmd":"set_mode","mode":5}` |
| JUGGLE | `{"cmd":"set_mode","mode":6}` |
| BPM | `{"cmd":"set_mode","mode":7}` |
| SNOW | `{"cmd":"set_mode","mode":8}` |
| COMET | `{"cmd":"set_mode","mode":9}` |
| RAINBOW_GLITTER | `{"cmd":"set_mode","mode":10}` |
| COLOR_WAVES | `{"cmd":"set_mode","mode":11}` |
| THEATER_CHASE | `{"cmd":"set_mode","mode":12}` |
| SOLID_GLOW | `{"cmd":"set_mode","mode":13}` |
| Set brightness (0-255) | `{"cmd":"set_brightness","value":128}` |
| Set LED count (1-256) | `{"cmd":"set_led_count","value":60}` |
| Configure WiFi | `{"cmd":"set_wifi","ssid":"Name","pass":"pass"}` |

---

## Notes

1. **Command order**: To turn on the LED in a specific mode, first send the `set_power` command with `state:1`, then `set_mode` with the desired mode.

2. **Auto-save**: All settings are automatically saved to device memory and restored after reboot.

3. **IP address**: Replace `192.168.1.100` with the actual IP address of your ESP32 device.

4. **Ports**: 
   - WebSocket typically runs on port `81`
   - UDP typically runs on port `3333`
   - Check the settings in your device code

5. **Delays**: When sending multiple commands in succession, it's recommended to add small pauses (100-200 ms) between commands.

6. **Mode range**: Valid mode values are `0` to `13` (13 effects total).

7. **Brightness**: Values range from `0` (off) to `255` (maximum). Default is `51` (~20%).

8. **LED count**: Valid range is `1` to `256`. Default is `30`.
