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
#include <string.h>

// For Functions
OpenWeatherMapCurrent client;
String OPEN_WEATHER_MAP_APP_ID = "a8d102311281b30cfe15e9eafd971be5";
String OPEN_WEATHER_MAP_LOCATION_ID = "1839726"; // ASAN ID
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;
WiFiClient wifiClient;

// WIFI Settings
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "iptime2.4G-301BB";
const char* WIFI_PASSWORD = "zjarhd301";

// MyVariable
OpenWeatherMapCurrentData data; // API data를 담을 변수
int redPin = 13;
int greenPin = 12;
int bluePin = 15;
int rainPin = 16;
int earthPin = 5;
int sunPin = 4;
int moonPin = 0;

// MyFunctions
void connectWifi(); // 와이파이 연결
void getData(); // API 정보 받아와서 data에 저장
void printInfoSample(); // data에서 몇가지 정보를 출력해보기
void setColor(int red, int green, int blue); // RGB 제어하기
void weatherRGBControl(); // 날씨에 맞게 rgb를 제어해줌
void rainLEDControl(int doOn);
void earthLEDControl(int doOn);
void sunLEDControl(int doOn);
void moonLEDControl(int doOn);
String getTimes();
void timeManager();

void setup() {
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(rainPin, OUTPUT);
  pinMode(earthPin, OUTPUT);
  pinMode(sunPin, OUTPUT);
  pinMode(moonPin, OUTPUT);
  delay(500);
  connectWifi();
  setColor(255, 255, 255); // 초기화
  rainLEDControl(0); // 비 꺼지게
  earthLEDControl(0); // 지구 꺼지게
  sunLEDControl(0); // 태양 꺼지게
  moonLEDControl(0); // 달 꺼지게
}

void loop() {
  getData(); // 날씨 정보 받아오기
  delay(500);
  printInfoSample(); // 가져온 정보 출력
  delay(500);
  weatherRGBControl(); // 현재 날씨에 따라서 RGB 제어하기
  delay(500);
  timeManager();
  delay(60000); // 1분 지연
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
  
  Serial.println();
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

void weatherRGBControl() {
  const char *weather = data.main.c_str();

  if ( strcmp(weather, "Thunderstorm") == 0 ) { // 천둥번개
    setColor(14, 59, 240); // 찐 노란색
    rainLEDControl(0);
    Serial.println("천둥 번개 상태");
    return;
  }
  if ( strcmp(weather, "Drizzle") == 0 ) { // 이슬비
    setColor(255, 255, 0);
    rainLEDControl(1);
    Serial.println("이슬비 상태");
    return;
  }
  if ( strcmp(weather, "Rain") == 0 ) { // 비
    setColor(255, 255, 0);
    rainLEDControl(1);
    Serial.println("비 상태");
    return;
  }
  if ( strcmp(weather, "Snow") == 0 ) { // 눈
    setColor(255, 255, 0);
    rainLEDControl(1);
    Serial.println("눈 상태");
    return;
  }
  if ( strcmp(weather, "Clear") == 0 ) { // 맑음
    setColor(19, 15, 14); // 맑음
    rainLEDControl(0);
    Serial.println("맑음 상태");
    return;
  }
  if ( strcmp(weather, "Clouds") == 0 ) { // 구름 낌
    setColor(203, 103, 36); // 하늘색
    rainLEDControl(0);
    Serial.println("구름 낌 상태");
    return;
  }
  Serial.println("예외 상태"); // Mist, Smoke, Haze, Dust, Fog, Sand, Ash, Squall, Tornado
  setColor(150, 166, 73); // 약간 회색느낌?
  rainLEDControl(0);
  return;
}

void rainLEDControl(int doOn) {
  if (doOn == 1) {
    digitalWrite(rainPin, HIGH);
  }
  else {
    digitalWrite(rainPin, LOW);
  }
}

void earthLEDControl(int doOn) {
  if (doOn == 1) {
    digitalWrite(earthPin, HIGH);
  }
  else {
    digitalWrite(earthPin, LOW);
  }
}

void sunLEDControl(int doOn) {
  if (doOn == 1) {
    digitalWrite(sunPin, HIGH);
  }
  else {
    digitalWrite(sunPin, LOW);
  }
}

void moonLEDControl(int doOn) {
  if (doOn == 1) {
    digitalWrite(moonPin, HIGH);
  }
  else {
    digitalWrite(moonPin, LOW);
  }
}

void timeManager() {
  String time_data = getTimes();
  char time_a = time_data[17];
  char time_b = time_data[18];

  Serial.println(time_a);
  Serial.println(time_b);

  /*
   * 오후 6시부터 오전 6시전까지 불 켜지게
   * +9시간 시차 고려
   * 오전 9시부터 오후 9시
   */

   if (time_a == '0') {
    if (time_b >= '9') {
      earthLEDControl(1);
      sunLEDControl(0);
      moonLEDControl(1);
    }
    else {
      earthLEDControl(0);
      sunLEDControl(1);
      moonLEDControl(0);
    }
   }
   else if (time_a == '1') {
    earthLEDControl(1);
    sunLEDControl(0);
    moonLEDControl(1);
   }
   else if (time_a == '2') {
    if (time_b < '1') {
      earthLEDControl(1);
      sunLEDControl(0);
      moonLEDControl(1);
    }
    else {
      earthLEDControl(0);
      sunLEDControl(1);
      moonLEDControl(0);
    }
   }
}

String getTimes() {
  WiFiClient client;
  while (!!!client.connect("google.com", 80)) { //  google.com 연결 안되면, !!! 또는 ! 는 부울
  Serial.println("connection failed, retrying..."); 
  }
  
  client.print("HEAD / HTTP/1.1\r\n\r\n"); // google.com 에 접속해서 헤더파일 받아오는 명령어?

  while(!!!client.available()) {} // !!! 또는 ! 는 부울

  while(client.available()){
    if (client.read() == '\n') {   
      if (client.read() == 'D') {   
        if (client.read() == 'a') {   
          if (client.read() == 't') {   
            if (client.read() == 'e') {   
              if (client.read() == ':') {   
                client.read();
                String theDate = client.readStringUntil('\r'); 
                client.stop();
                return theDate;  // Date: 까지 읽고, 맞으면 \r 까지 읽어오기.
              }
            }
          }
        }
      }
    }
  }
}
