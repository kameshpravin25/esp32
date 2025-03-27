#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <esp_netif.h>
#include <esp_netif_types.h>
#include <math.h>
#include <vector>

const char* ssid = "esp32";
const char* password = "12345678";

WebServer server(80);

// Structure declaration
struct DeviceInfo {
  String mac;
  String ip;
};

// Global declarations
std::vector<DeviceInfo> connectedDevices;
void WiFiEvent(arduino_event_id_t event, arduino_event_info_t info);

// ================================================================
// WiFi Event Handler
// ================================================================
void WiFiEvent(arduino_event_id_t event, arduino_event_info_t info) {
  char macStr[18];

  switch (event) {
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
               info.wifi_ap_staconnected.mac[0],
               info.wifi_ap_staconnected.mac[1],
               info.wifi_ap_staconnected.mac[2],
               info.wifi_ap_staconnected.mac[3],
               info.wifi_ap_staconnected.mac[4],
               info.wifi_ap_staconnected.mac[5]);
      connectedDevices.push_back({macStr, "N/A"});
      break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
               info.wifi_ap_stadisconnected.mac[0],
               info.wifi_ap_stadisconnected.mac[1],
               info.wifi_ap_stadisconnected.mac[2],
               info.wifi_ap_stadisconnected.mac[3],
               info.wifi_ap_stadisconnected.mac[4],
               info.wifi_ap_stadisconnected.mac[5]);
      for (auto it = connectedDevices.begin(); it != connectedDevices.end(); ++it) {
        if (it->mac == macStr) {
          connectedDevices.erase(it);
          break;
        }
      }
      break;
      
    default: break;
  }
}

// ================================================================
// Device Data Helpers
// ================================================================
String getConnectedDevicesJSON() {
  String json = "[";
  for (size_t i = 0; i < connectedDevices.size(); i++) {
    if (i > 0) json += ",";
    json += "{\"mac\":\"" + connectedDevices[i].mac + "\",";
    json += "\"ip\":\"" + connectedDevices[i].ip + "\"}";
  }
  json += "]";
  return json;
}

void updateDeviceIPs() {
  wifi_sta_list_t wifi_sta_list;
  esp_netif_sta_list_t netif_sta_list;
  
  if (esp_wifi_ap_get_sta_list(&wifi_sta_list) == ESP_OK &&
      esp_netif_get_sta_list(&wifi_sta_list, &netif_sta_list) == ESP_OK) {
    for (int i = 0; i < netif_sta_list.num; i++) {
      char mac[18], ip[16];
      snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
               netif_sta_list.sta[i].mac[0],
               netif_sta_list.sta[i].mac[1],
               netif_sta_list.sta[i].mac[2],
               netif_sta_list.sta[i].mac[3],
               netif_sta_list.sta[i].mac[4],
               netif_sta_list.sta[i].mac[5]);
      snprintf(ip, sizeof(ip), IPSTR, IP2STR(&netif_sta_list.sta[i].ip));
      
      for (auto& dev : connectedDevices) {
        if (dev.mac == mac) {
          dev.ip = ip;
          break;
        }
      }
    }
  }
}

// ================================================================
// Web Server Handlers
// ================================================================
String buildDashboard() {
  updateDeviceIPs();
  
  String page = R"(
<!DOCTYPE html><html><head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Network Monitor</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
<style>
:root { --primary: #2c3e50; --secondary: #3498db; --background: #f5f6fa; }
body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin: 0; background: var(--background); }
.container { max-width: 800px; margin: 20px auto; padding: 20px; }
.header { text-align: center; margin-bottom: 30px; }
.header h1 { color: var(--primary); margin: 0; font-size: 2.5em; }
.dashboard { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; margin-top: 30px; }
.card { background: white; border-radius: 15px; padding: 20px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
.network-map { position: relative; height: 400px; background: linear-gradient(145deg, #ffffff, #f8f9fa); border-radius: 15px; }
.router-node { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); 
  width: 80px; height: 80px; border-radius: 50%; background: var(--secondary); 
  display: flex; align-items: center; justify-content: center; color: white; 
  box-shadow: 0 0 20px rgba(52,152,219,0.3); animation: pulse 2s infinite; }
.device-node { position: absolute; width: 50px; height: 50px; border-radius: 50%; 
  background: #e74c3c; display: flex; align-items: center; justify-content: center; 
  color: white; transition: all 0.3s ease; cursor: pointer; }
.device-node:hover { transform: scale(1.1); box-shadow: 0 0 15px rgba(231,76,60,0.4); }
.device-node i { font-size: 1.2em; }
.device-tooltip { position: absolute; background: rgba(0,0,0,0.8); color: white; 
  padding: 8px 12px; border-radius: 6px; font-size: 0.9em; white-space: nowrap; 
  pointer-events: none; opacity: 0; transition: opacity 0.2s; }
@keyframes pulse { 
  0% { transform: translate(-50%, -50%) scale(0.95); } 
  50% { transform: translate(-50%, -50%) scale(1.05); } 
  100% { transform: translate(-50%, -50%) scale(0.95); } 
}
.stats { display: grid; gap: 15px; }
.stat-item { display: flex; justify-content: space-between; align-items: center; padding: 15px; background: #f8f9fa; border-radius: 10px; }
.stat-label { color: #7f8c8d; }
.stat-value { font-weight: bold; color: var(--primary); }
</style>
</head>
<body>
<div class="container">
  <div class="header">
    <h1>📶 Network Monitor</h1>
    <p>Connected to ESP32 Access Point</p>
  </div>
  <div class="dashboard">
    <div class="card network-map" id="networkMap">
      <div class="router-node"><i class="fas fa-wifi fa-2x"></i></div>
    </div>
    <div class="stats">
      <div class="card">
        <div class="stat-item"><span class="stat-label">Access Point:</span><span class="stat-value">)";
  page += ssid;
  page += R"(</span></div>
        <div class="stat-item"><span class="stat-label">Connected Devices:</span><span class="stat-value" id="deviceCount">0</span></div>
        <div class="stat-item"><span class="stat-label">AP IP Address:</span><span class="stat-value">)";
  page += WiFi.softAPIP().toString();
  page += R"(</span></div>
      </div>
    </div>
  </div>
</div>
<script>
let rotation = 0;
function updateDevices(devices) {
  const map = document.getElementById('networkMap');
  map.innerHTML = '<div class="router-node"><i class="fas fa-wifi fa-2x"></i></div>';
  document.getElementById('deviceCount').textContent = devices.length;
  const radius = 150;
  const centerX = map.offsetWidth / 2;
  const centerY = map.offsetHeight / 2;
  
  devices.forEach((device, index) => {
    const angle = (index * (360 / devices.length) + rotation) * (Math.PI / 180);
    const x = centerX + radius * Math.cos(angle) - 25;
    const y = centerY + radius * Math.sin(angle) - 25;
    
    const node = document.createElement('div');
    node.className = 'device-node';
    node.style.left = x + 'px';
    node.style.top = y + 'px';
    node.innerHTML = '<i class="fas fa-mobile-alt"></i>';
    
    const tooltip = document.createElement('div');
    tooltip.className = 'device-tooltip';
    tooltip.textContent = ${device.mac}\n${device.ip};
    node.appendChild(tooltip);
    
    node.addEventListener('mousemove', (e) => {
      tooltip.style.left = ${e.pageX + 10}px;
      tooltip.style.top = ${e.pageY + 10}px;
      tooltip.style.opacity = '1';
    });
    
    node.addEventListener('mouseleave', () => {
      tooltip.style.opacity = '0';
    });
    
    map.appendChild(node);
  });
  
  rotation = (rotation + 0.5) % 360;
}

async function fetchDevices() {
  try {
    const response = await fetch('/devices');
    const devices = await response.json();
    updateDevices(devices);
  } catch (error) {
    console.error('Error:', error);
  }
}

setInterval(fetchDevices, 2000);
</script>
</body>
</html>)";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", buildDashboard());
}

void handleDevices() {
  server.send(200, "application/json", getConnectedDevicesJSON());
}

// ================================================================
// Main Program
// ================================================================
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.onEvent(WiFiEvent);
  
  server.on("/", handleRoot);
  server.on("/devices", handleDevices);
  server.begin();
}

void loop() {
  server.handleClient();
}
