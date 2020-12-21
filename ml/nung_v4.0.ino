int sensorPin = 16;
int sensorValue;

#include <ESP8266WiFi.h>
#include "WiFiClient.h"
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "YeeChouIE"
#define WIFI_PASSWORD "27643996"

#define FIREBASE_HOST "project-nung.firebaseio.com"
#define FIREBASE_AUTH "mj2pw6D57SwHe0gpB8AQNyAliMFMPkbjiQjQQpsz"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

unsigned long previousMillis = 0;
const long interval = 1800000;

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(sensorPin, INPUT);
  Serial.println();
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("WiFi is down");
  delay(20000);

  WiFi.forceSleepWake();
  delay(1);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  timeClient.setTimeOffset(28800);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  long v = 0;
  unsigned long currentMillis = millis();

  while (1) {
    previousMillis = currentMillis;
    v = getVibration();

    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    String t = timeClient.getFormattedTime();

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["timestamp"] = t;
    root["vibration"] = v;

    String data = Firebase.push("/data", root);
    if (Firebase.failed()) {
      Serial.print("Failed to push data to Firebase");
      Serial.println(Firebase.error());
      return;
    }
    else {
      Serial.print("Successfully pushed data to Firebase");
      Serial.println(data);
    }
  }
}

long getVibration() {
  long vibSum = 0;
  long vib = 0;
  long avgVib = 0;
  int N = 6;
  int readings[N];

  for (int i = 0; i < 30; i++) {
    vib = pulseIn(sensorPin, HIGH);
    readings[i] = vib;
    delay(300000);
  }

  for (int j = 0; j < N; j++) {
    vibSum = vibSum + readings[j];
    Serial.println(readings[j]);
  }

  Serial.println("vibSum: ");
  Serial.println(vibSum);
  avgVib = vibSum / N;
  Serial.println(avgVib);

  return avgVib;
}
