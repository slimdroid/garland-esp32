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

### Mode 0: FADE (smooth color transitions)
```json
{"cmd":"set_mode","mode":0}
```

### Mode 1: RED (red)
```json
{"cmd":"set_mode","mode":1}
```

### Mode 2: GREEN (green)
```json
{"cmd":"set_mode","mode":2}
```

### Mode 3: BLUE (blue)
```json
{"cmd":"set_mode","mode":3}
```

### Mode 4: WHITE (white)
```json
{"cmd":"set_mode","mode":4}
```

---

## Typical Usage Scenarios

### Scenario 1: Turn on LED in red mode
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":1}
```

### Scenario 2: Switch to green color
```json
{"cmd":"set_mode","mode":2}
```

### Scenario 3: Enable smooth color transition mode
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":0}
```

### Scenario 4: Turn off LED
```json
{"cmd":"set_power","state":0}
```

### Scenario 5: Turn on white light
```json
{"cmd":"set_power","state":1}
{"cmd":"set_mode","mode":4}
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
    
    // Set red color
    ws.send('{"cmd":"set_mode","mode":1}');
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

// Usage examples:
// turnOn();           // Turn on
// setMode(1);         // Red
// setMode(2);         // Green
// setMode(3);         // Blue
// setMode(4);         // White
// setMode(0);         // Smooth transition
// turnOff();          // Turn off
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

# Turn on LED
send_command('{"cmd":"set_power","state":1}')
time.sleep(0.1)

# Set red color
send_command('{"cmd":"set_mode","mode":1}')
time.sleep(2)

# Switch to green
send_command('{"cmd":"set_mode","mode":2}')
time.sleep(2)

# Switch to blue
send_command('{"cmd":"set_mode","mode":3}')
time.sleep(2)

# Switch to white
send_command('{"cmd":"set_mode","mode":4}')
time.sleep(2)

# Smooth color transition mode
send_command('{"cmd":"set_mode","mode":0}')
time.sleep(5)

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

# Set red color
echo '{"cmd":"set_mode","mode":1}' | nc -u $DEVICE_IP $PORT

# Set green color
echo '{"cmd":"set_mode","mode":2}' | nc -u $DEVICE_IP $PORT

# Set blue color
echo '{"cmd":"set_mode","mode":3}' | nc -u $DEVICE_IP $PORT

# Set white color
echo '{"cmd":"set_mode","mode":4}' | nc -u $DEVICE_IP $PORT

# Smooth color transition mode
echo '{"cmd":"set_mode","mode":0}' | nc -u $DEVICE_IP $PORT

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

# Turn on LED
Send-Command '{"cmd":"set_power","state":1}'
Start-Sleep -Milliseconds 100

# Set red color
Send-Command '{"cmd":"set_mode","mode":1}'
Start-Sleep -Seconds 2

# Switch to green
Send-Command '{"cmd":"set_mode","mode":2}'
Start-Sleep -Seconds 2

# Switch to blue
Send-Command '{"cmd":"set_mode","mode":3}'
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
| Smooth color transition | `{"cmd":"set_mode","mode":0}` |
| Red | `{"cmd":"set_mode","mode":1}` |
| Green | `{"cmd":"set_mode","mode":2}` |
| Blue | `{"cmd":"set_mode","mode":3}` |
| White | `{"cmd":"set_mode","mode":4}` |
| Get status | `{"cmd":"get_status"}` |

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
