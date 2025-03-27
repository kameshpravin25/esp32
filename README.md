# ESP32 Network Monitor

This project sets up an **ESP32 as a Wi-Fi access point** and monitors connected devices. It provides a **web-based dashboard** to display connected devices' MAC addresses and IPs in a **network map visualization**.

## Features

- **ESP32 Access Point**: Creates a local Wi-Fi network.
- **Device Monitoring**: Tracks connected devices (MAC & IP).
- **Web Dashboard**: Displays connected devices in an interactive UI.
- **Live Updates**: Fetches device details dynamically every 2 seconds.

## Hardware Requirements

- ESP32 Development Board
- USB Cable for Power & Programming
- Computer with Arduino IDE or PlatformIO

## Software Requirements

- **Arduino IDE** (or **PlatformIO**)
- **ESP32 Board Support Package**
- **WiFi.h**, **WebServer.h**, **esp\_wifi.h**, **esp\_netif.h**

## Installation & Setup

1. **Clone the Repository**
   ```sh
   git clone https://github.com/yourusername/ESP32-Network-Monitor.git
   cd ESP32-Network-Monitor
   ```
2. **Open the Project in Arduino IDE**
3. **Install ESP32 Board Support** (if not already installed):
   - Go to `File > Preferences`
   - Add `https://dl.espressif.com/dl/package_esp32_index.json` under **Additional Board Manager URLs**
   - Open `Board Manager` and install **ESP32 by Espressif Systems**
4. **Connect Your ESP32 Board & Select the Correct Port**
5. **Upload the Code** to the ESP32
6. **Connect to the ESP32 Network (**``**)**
7. **Open the Web Dashboard** at `http://192.168.4.1`

## Web Dashboard

The dashboard features:

- **Network Map**: Displays connected devices in a circular arrangement around the ESP32 AP.
- **Device Stats**: Shows total connected devices and AP details.
- **Live Data Fetching**: Updates device connections in real-time.

## API Endpoints

| Endpoint   | Method | Description                              |
| ---------- | ------ | ---------------------------------------- |
| `/`        | GET    | Returns the web dashboard                |
| `/devices` | GET    | Returns a JSON list of connected devices |

## Example API Response

```json
[
  {"mac": "A4:5E:60:3B:8A:1C", "ip": "192.168.4.2"},
  {"mac": "F8:1A:67:4D:3B:5F", "ip": "192.168.4.3"}
]
```

## License

This project is licensed under the **MIT License**. Feel free to use and modify it.

---

Developed by **Your Name**

Kamesh Pravin
