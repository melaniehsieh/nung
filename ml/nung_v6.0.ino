/*
   Melanie Hsieh
   Project Nung
   Ver. 6.0 (2021-Feb-07)
   NO AutoConnect Version
   Board: LILYGO® TTGO T-OI ESP8266 with Mini D1 Board
   Windows USB Interface Driver: http://www.wch.cn/download/CH341SER_EXE.html
   MacOS USB Interface Driver:   http://www.wch.cn/download/CH341SER_MAC_ZIP.html
   Battery: 16340 Li-Ion NicJoy 750mAh 3.7V
*/

#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <time.h>
#include <WiFiClient.h>
#include <FirebaseArduino.h> // https://github.com/FirebaseExtended/firebase-arduino
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>     // Ver. 5.13.5 by Benoit Blanchon

#define WIFI_SSID "YeeChouIE"
#define WIFI_PASSWORD "27643996"
#define FIREBASE_HOST "project-nung.firebaseio.com"
#define FIREBASE_AUTH "mj2pw6D57SwHe0gpB8AQNyAliMFMPkbjiQjQQpsz"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Sensor

int sensorPin = 16;
int sensorValue;

// Timer
unsigned long previousMillis = 0;
const long interval = 2UL * 1000UL; // every 2 seconds
const long AvgValueInterval = 5UL * 60UL * 1000UL; // every 5 Minutes (if changes, divider variable must change too
const long TransmissionInterval = 30UL * 60UL * 1000UL; // every 30 Minutes
const long resetcounter = 12UL * 60UL * 60UL * 1000UL; // restart every 12 hours

// Variables
long vib = 0; // Vibration Value
long vibSum = 0;  // Sum of Vibrations added durin one loop
long avgVib = 0;  // Sum of Vobrations divided by the number of measurements taken
long avgSum = 0;  // Sum of the avergare vibrations during a 30 min. cycle
const int divider = 150; // 5 minutes = 300 seconds, divided by measurement every 2 seconds
long x[divider];  // Array that holds all measurements till an average vibSum is calculated
const int y_divider = 6; // Number of cycles to collect avgSum
long y[y_divider]; // Array that holds the average 30 minutes measurements
int xCount = 0; // Counter to keep track of vibSum cycle
int yCount = 0; // counter to keep track of avgSum cycle
int transmissionSum = 0; // variable to hold the final value that will be transmitted
int wifi_connect_error = 0; // count connection attempts
const int wifi_timeout = 100; // max attempts


void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(10);
  pinMode(sensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Green LED (WiFi)
  digitalWrite(LED_BUILTIN, LOW); // LOW = ON; HIGH = OFF
  Serial.println();
  Serial.println();

  timeClient.begin();
  timeClient.setTimeOffset(28800);

  WiFi_Off();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  WiFi_On();
}


void loop() {
  unsigned long currentMillis = millis();

  // grab value from sensor every 2 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis += interval;
    vib = pulseIn(sensorPin, HIGH);
    x[xCount] = vib;
    xCount += 1;
    Serial.print(".");
  }

  // calculate 5-min avergae value
  if (xCount == divider) {
    for (int i = 0; i < divider; i++) {
      vibSum = vibSum + x[i];
      Serial.println(x[i]);
    }
    Serial.print("vibSum: ");
    Serial.println(vibSum);
    avgVib = vibSum / divider;
    y[yCount] = avgVib;
    yCount += 1;
    xCount = 0;
    vibSum = 0;
    Serial.print("avgVib: ");
    Serial.println(avgVib);
    Serial.print("yCount: ");
    Serial.println(yCount);
  }

  // 30-min avergae value calculation transmission to Firebase DB
  if (yCount == y_divider) {
    for (int j = 0; j < y_divider; j++) {
      avgSum = avgSum + y[j];
      Serial.println(y[j]);
    }
    Serial.print("avgSum: ");
    Serial.println(avgSum);
    transmissionSum = avgSum / y_divider;
    yCount = 0;
    xCount = 0;
    Serial.print("transmission Sum: ");
    Serial.println(transmissionSum);
    TransmitData(transmissionSum);
    transmissionSum = 0;
    avgSum = 0;
  }

  // Reset ESP every 12 hours to keep timer in sync
  if (currentMillis >= resetcounter) {
    Serial.println("resetting");
    ESP.reset();
  }
}

bool WiFi_Off() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  Serial.print("WiFi shutting down");
  while (WiFi.status() == WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" [done]");
    digitalWrite(LED_BUILTIN, HIGH);
    return (true);
  } else {
    Serial.println(" [error shutting down]");
    digitalWrite(LED_BUILTIN, LOW);
    return (false);
  }
}

bool WiFi_On() {
  Serial.print("WiFi starting up");
  WiFi.forceSleepWake();
  WiFi.mode(WIFI_STA);
  //wifi_station_connect();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    if (wifi_connect_error >= wifi_timeout) {
      Serial.println("error connecting to wifi- upload skipped");
      wifi_connect_error = 0;
      return (false);
    }
    wifi_connect_error += 1;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" [done]");
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    digitalWrite(LED_BUILTIN, LOW);
    return (true);
  } else {
    Serial.println(" [error starting up]");
    digitalWrite(LED_BUILTIN, HIGH);
    return (false);
  }
}

bool TransmitData(long v) {
  WiFi_On();
  delay(5000);

  Serial.println("Updating Time: ");
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  String tString = timeClient.getFormattedTime();
  Serial.print("timestamp: ");
  Serial.println(tString);
  Serial.println("submitting data to Firebase");
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["timestamp"] = tString;
  root["vibration"] = v;
  root.prettyPrintTo(Serial); // Prints Json Object to the Serial Output
  String data = Firebase.push("/chak", root);
  if (Firebase.failed()) {
    Serial.println("Failed to push data to Firebase");
    Serial.println(Firebase.error());
  }
  else {
    Serial.println("Successfully pushed data to Firebase");
    Serial.println(data);
  }
  delay(2000);
  WiFi_Off();
  return (true);
}
