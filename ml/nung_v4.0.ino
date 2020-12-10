int sensorPin = 16;
int sensorValue;

#include <ESP8266WiFi.h>
#include "WiFiClient.h"
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "xxx"
#define WIFI_PASSWORD "xxx"

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
  Serial.println(v);
  Firebase.pushInt("vibration", v);
<<<<<<< HEAD:ml/nung_v4.0.ino
  
=======

>>>>>>> 82cde970cd865a0c1ae96987862b1425463a1997:ml/modem_sleep.ino
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String timestamp = timeClient.getFormattedTime();
  Serial.println(timestamp);
<<<<<<< HEAD:ml/nung_v4.0.ino
  
  Firebase.pushString("timestamp", timestamp);
  
=======

  Firebase.pushString("timestamp", timestamp);

>>>>>>> 82cde970cd865a0c1ae96987862b1425463a1997:ml/modem_sleep.ino
  if (Firebase.failed()) {
    Serial.print("pushing failed:");
    Serial.println(Firebase.error());
    return;
  }
<<<<<<< HEAD:ml/nung_v4.0.ino
  delay(1800000);
=======
  delay(1000);
>>>>>>> 82cde970cd865a0c1ae96987862b1425463a1997:ml/modem_sleep.ino
}


long getVibration() {

  long vibrations[6];
  long vibrationsSum = 0.00;
  for (int i = 0; i <= 5; i++) {
    vibrations[i] = pulseIn(sensorPin, HIGH);
    vibrationsSum = vibrationsSum + vibrations[i];
    delay(300000);
  }
  long averageVibrations = vibrationsSum / 6;
  return averageVibrations;
}
