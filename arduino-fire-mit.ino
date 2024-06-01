#define trigPin 6
#define echoPin 5

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "Sonny Mobile WIFI"
#define WIFI_PASSWORD "Ask4you!"
#define FIREBASE_HOST "https://mit-app-638ec-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "GEZm7tpQcbYpDqmpZ5IMgTjVPA1TNPdQM8o6DX9V"

char wifiStatus[20];

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  strcpy(wifiStatus, "Connected");
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    strcpy(wifiStatus, "Disconnected");
  } else {
    strcpy(wifiStatus, "Connected");
  }
  Serial.print("WiFi: ");
  Serial.println(wifiStatus);

  float distance = 0.01723 * readUltrasonicDistance(trigPin, echoPin);

  if (distance < 7) {
    Serial.println("WARNING: Object is less than 7cm away!");
    if (Firebase.setFloat("distance", distance)) {
      Serial.println("Distance uploaded to Firebase!");
    } else {
      Serial.print("Error uploading distance: ");
      Serial.println(Firebase.error());
    }
  }

  Serial.println(distance);

  delay(1000);
}

long readUltrasonicDistance(int trig, int echo) {
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH);
}

