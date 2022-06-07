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
int redPin = 13;
int greenPin = 12;
int bluePin = 15;

// MyFunctions
void connectWifi(); // 와이파이 연결
void getData(); // API 정보 받아와서 data에 저장
void printInfoSample(); // data에서 몇가지 정보를 출력해보기
void setColor(int red, int green, int blue); // RGB 제어하기

void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  delay(500);
  setColor(255, 255, 255);
  connectWifi();
}

void loop() {
  getData(); // 날씨 정보 받아오기
  delay(500);
  printInfoSample(); // 가져온 정보 출력
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

void printInfoSample() {
  // "main": "Rain", String main;
  Serial.printf("main: %s\n", data.main.c_str());
  // "description": "shower rain", String description;
  Serial.printf("description: %s\n", data.description.c_str());
  // "temp": 290.56, float temp;
  Serial.printf("temp: %f\n", data.temp);
  // "temp_min": 289.15, float tempMin;
  Serial.printf("tempMin: %f\n", data.tempMin);
  // "temp_max": 292.15 float tempMax;
  Serial.printf("tempMax: %f\n", data.tempMax);
  // "clouds": {"all": 90}, uint8_t clouds;
  Serial.printf("clouds: %d\n", data.clouds);
  // "dt": 1527015000, uint64_t observationTime;
  time_t time = data.observationTime;
  Serial.printf("observationTime: %d, full date: %s", data.observationTime, ctime(&time));
  // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", data.cityName.c_str());
  Serial.println();
  Serial.println("---------------------------------------------------/\n");
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue); 
}
