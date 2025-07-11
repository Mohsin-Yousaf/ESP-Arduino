#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid = "Tanbits";
const char* password = "Tanbits@10";

// OpenWeatherMap
const String city = "Lahore,PK";
const String apiKey = "YOUR_OPENWEATHERMAP_API_KEY";  // Get from https://openweathermap.org/api
const char* host = "api.openweathermap.org";

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 5 * 3600, 60000);  // UTC+5 for Pakistan

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.println(WiFi.localIP());

  timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.println("Time: " + timeClient.getFormattedTime());

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed to weather API");
    return;
  }

  String url = "/data/2.5/weather?q=" + city + "&appid=" + apiKey + "&units=metric";

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  delay(1000);

  String line;
  while (client.available()) {
    line = client.readStringUntil('\n');
    if (line.indexOf("\"temp\":") >= 0) {
      int start = line.indexOf(":") + 1;
      int end = line.indexOf(",", start);
      String temp = line.substring(start, end);
      Serial.println("Temperature in Lahore: " + temp + " °C");
      break;
    }
  }

  Serial.println("---------------------------");
  delay(60000); // wait 1 minute before next update
}
