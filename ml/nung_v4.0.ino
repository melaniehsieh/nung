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
  long v = getVibration();
  Firebase.pushInt("vibration", v);

  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String timestamp = timeClient.getFormattedTime();

  Firebase.pushString("timestamp", timestamp);

  if (Firebase.failed()) {
    Serial.print("pushing failed:");
    Serial.println(Firebase.error());
    return;
  }
  delay(1800000);
}

long getVibration() {
  long vibSum = 0.00;
  long vib = 0.00;
  for (int i = 0; i <= 5; i++) {
    vib = pulseIn(sensorPin, HIGH);
    vibSum = vibSum + vib;
    delay(300000);
  }
  long avgVib = vibSum / 6;
  Serial.println(avgVib);

  return avgVib;
}
