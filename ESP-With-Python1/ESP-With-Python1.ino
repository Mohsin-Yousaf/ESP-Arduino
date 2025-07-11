#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Tanbits";
const char* password = "Tanbits@10";

const String serverUrl = "http://192.168.4.191:5000/get_relay";  // Replace with your PC's IP

const int ledPin = LED_BUILTIN;  // Usually GPIO2 (inverted logic: LOW = ON, HIGH = OFF)

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Turn off LED initially (inverted logic)

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;  // Create a WiFi client
    HTTPClient http;

    http.begin(client, serverUrl);  // Use the updated begin()
    int httpCode = http.GET();


    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println("Received: " + payload);

      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        int Breaker1 = doc["Breaker1"];
        if (Breaker1 == 1) {
          digitalWrite(ledPin, LOW); // ON
        } else {
          digitalWrite(ledPin, HIGH); // OFF
        }
      } else {
        Serial.println("JSON parse error");
      }
    } else {
      Serial.printf("HTTP error: %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(1000); // Check every second
}
