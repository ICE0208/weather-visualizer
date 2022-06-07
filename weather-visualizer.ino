/*
 * WEATHER VISUALIZER
 * BOARD : NodeMCU 1.0 (ESP-12E Module)
 * UPLOAD SPEED : 115200
 */
// Header Files
#include <Arduino.h>
#include <ESP8266WiFi.h>

// For Functions
WiFiClient wifiClient;

// WIFI Settings
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "iptime2.4G-410";
const char* WIFI_PASSWORD = "zjarhd410";

void connectWifi(); // 와이파이 연결

void setup() {
  Serial.begin(115200);
  delay(500);
  connectWifi();
}

void loop() {

}

void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
  Serial.println();
}
