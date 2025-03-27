#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// AP and MQTT broker settings
#define WLAN_SSID       "esp32"
#define WLAN_PASS       "12345678"
#define MQTT_SERVER     "192.168.4.1"  // ESP32's AP IP (default for softAP)
#define MQTT_SERVERPORT 1883

// Authorized credentials:
#define MQTT_USERNAME "validuser"
#define MQTT_PASSWORD "validpass"

// WiFi and MQTT client setup
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// Define a publish topic
Adafruit_MQTT_Publish authorizedTopic = Adafruit_MQTT_Publish(&mqtt, "test/topic");

void setup() {
  Serial.begin(115200);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting to AP");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to ESP32 AP");

  // Attempt to connect to MQTT
  int8_t ret;
  if ((ret = mqtt.connect()) != 0) {
    Serial.print("MQTT connection failed with code: ");
    Serial.println(ret);
    // Retry or handle connection failure as needed
    while(1);
  }
  Serial.println("Authorized MQTT client connected.");

  // Publish a test message
  if (authorizedTopic.publish("Hello from authorized client")) {
    Serial.println("Message published successfully.");
  } else {
    Serial.println("Failed to publish message.");
  }
}

void loop() {
  // Additional publish/subscribe logic can be added here
}
