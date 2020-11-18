int sensorPin = 16;
int sensorValue;

#include <ESP8266WiFi.h>
#include "WiFiClient.h"
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "YeeChouIE"
#define WIFI_PASSWORD "27643996"

//#define FIREBASE_HOST "project-nung.firebaseio.com"
//#define FIREBASE_AUTH "mj2pw6D57SwHe0gpB8AQNyAliMFMPkbjiQjQQpsz"

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
  //  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  long v = getVibration();
  Serial.println(v);
  //  Firebase.pushInt("vibration", v);

  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String timestamp = timeClient.getFormattedTime();
  Serial.println(timestamp);

  //  Firebase.pushString("timestamp", timestamp);

  //  if (Firebase.failed()) {
  //    Serial.print("pushing failed:");
  //    Serial.println(Firebase.error());
  //    return;
  //  }
  delay(1000);
}


long getVibration() {
  long v = pulseIn(sensorPin, HIGH);
  return v;
}
