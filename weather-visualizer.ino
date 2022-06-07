/*
 * WEATHER VISUALIZER
 * BOARD : NodeMCU 1.0 (ESP-12E Module)
 * UPLOAD SPEED : 115200
 */
// Header Files
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <JsonListener.h>
#include <time.h>
#include "OpenWeatherMapCurrent.h"

// For Functions
OpenWeatherMapCurrent client;
String OPEN_WEATHER_MAP_APP_ID = "a8d102311281b30cfe15e9eafd971be5";
String OPEN_WEATHER_MAP_LOCATION_ID = "1839726"; // ASAN ID
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;
WiFiClient wifiClient;

// WIFI Settings
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "iptime2.4G-410";
const char* WIFI_PASSWORD = "zjarhd410";

// MyVariable
OpenWeatherMapCurrentData data; // API data를 담을 변수

// MyFunctions
void connectWifi(); // 와이파이 연결
void getData(); // API 정보 받아와서 data에 저장

void setup() {
  Serial.begin(115200);
  delay(500);
  connectWifi();
}

void loop() {
  getData(); // 날씨 정보 받아오기
  delay(500);
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

void getData() {
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
  Serial.println("------------------------------------");
}
