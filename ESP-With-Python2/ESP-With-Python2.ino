#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Tanbits";
const char* password = "Tanbits@10";

const String serverUrl = "http://192.168.4.191:5000/get_relay";  // Replace with your PC's IP

//const int ledPin = LED_BUILTIN;  // Usually GPIO2 (inverted logic: LOW = ON, HIGH = OFF)
const int BreakerPin1 = D5;
const int IsolatorPin1 = D6;

const int BreakerPin2 = D7;
const int IsolatorPin2 = D8;


void setup() {
  Serial.begin(115200);
//  pinMode(ledPin, OUTPUT);
  pinMode(BreakerPin1, OUTPUT);
  pinMode(IsolatorPin1, OUTPUT);
  pinMode(BreakerPin2, OUTPUT);
  pinMode(IsolatorPin2, OUTPUT);


//  digitalWrite(ledPin, HIGH); // Turn off LED initially (inverted logic)
  digitalWrite(BreakerPin1, LOW);
  digitalWrite(IsolatorPin1, LOW);
  digitalWrite(BreakerPin2, LOW);
  digitalWrite(IsolatorPin2, LOW);

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
        int Isolator1 = doc["Isolator1"];

        int Breaker2 = doc["Breaker2"];
        int Isolator2 = doc["Isolator2"];

////////////////////////   SET 1   //////////////////////
        if (Breaker1 == 1) {
          digitalWrite(BreakerPin1, HIGH); // ON
        } else {
          digitalWrite(BreakerPin1, LOW); // OFF
        }

        if (Isolator1 == 1) {
          digitalWrite(IsolatorPin1, HIGH); // ON
        } else {
          digitalWrite(IsolatorPin1, LOW); // OFF
        }

////////////////////////   SET 2   //////////////////////
        if (Breaker2 == 1) {
          digitalWrite(BreakerPin2, HIGH); // ON
        } else {
          digitalWrite(BreakerPin2, LOW); // OFF
        }

        if (Isolator2 == 1) {
          digitalWrite(IsolatorPin2, HIGH); // ON
        } else {
          digitalWrite(IsolatorPin2, LOW); // OFF
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
