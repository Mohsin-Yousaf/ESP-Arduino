#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Tanbits";
const char* password = "Tanbits@10";

const String getUrl = "http://192.168.4.191:5000/get_relay";
const String postUrl = "http://192.168.4.191:5000/relay_feedback"; // Define this on your server

const int ledPin = D6;
String serial_value = "";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // OFF initially (inverted logic)

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {

  if (Serial.available()) {
    serial_value = Serial.readStringUntil('\n'); // Read full word until newline
    serial_value.trim(); // Removes trailing newline or space
    Serial.println("Received: " + serial_value);
  }

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, getUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println("Received: " + payload);

      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        int Breaker1 = doc["Breaker1"];
        if (Breaker1 == 1 || serial_value == "high") {
          digitalWrite(ledPin, HIGH);  // ON
          Breaker1 = 1;
        } if (Breaker1 == 0 || serial_value == "low") {
          digitalWrite(ledPin, LOW); // OFF
        }
        serial_value = "";
        // Prepare feedback JSON
        int actualState = digitalRead(ledPin) == LOW ? 0 : 1;  // Inverted logic
        StaticJsonDocument<100> feedbackDoc;
        feedbackDoc["Breaker1"] = actualState;
        Serial.printf("actualState =  %d\n", actualState);

        String feedbackJson;
        serializeJson(feedbackDoc, feedbackJson);
        Serial.println("Sending feedback: " + feedbackJson);

        // Send POST feedback
        HTTPClient feedbackHttp;
        feedbackHttp.begin(client, postUrl);
        feedbackHttp.addHeader("Content-Type", "application/json");
        int postCode = feedbackHttp.POST(feedbackJson);

        if (postCode > 0) {
          Serial.printf("Feedback POST sent. Response code: %d\n", postCode);
        } else {
          Serial.printf("Failed to send feedback: %s\n", feedbackHttp.errorToString(postCode).c_str());
        }

        feedbackHttp.end();
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

  delay(1000); // Repeat every second
}
