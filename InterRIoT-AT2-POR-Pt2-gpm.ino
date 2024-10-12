/** 
 * Assessment Title: Portfolio Part 2
 * Cluster:          Intermediate RIoT  
 * Qualification:    ICT50220 Diploma of Information Technology (Advanced Programming) 
 * Name:             Gabriela Morales
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * This code is trying to connect to Adafruit's Free MQTT service, displaying a error message if is not connect and 
 * if is successful will display the wifi information.
 * 
 * Components & Identifiers: 
 * 
 */ 
 
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define IO_USERNAME "Gabriela07"
#define IO_KEY "aio_PvZZ255A8oKG9t2wATZNYzkEMN1c"
#define IO_SERVER "io.adafruit.com"
#define IO_SERVERPORT 1883

#define RETRY_PERIOD 10000 // 10 SECONDS
#define MAX_ATTEMPTS 4     
#define DOT_PER_LINE 5

const char* ssid = "TelstraA81499";         
const char* password = "3kq4mvdbec"; 

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

void setup() {
  Serial.begin(9600);


  bool connected = wiFiConnect();
  if (connected) {
    Serial.println("Wi-Fi Connected");
    wiFiDetails();

    bool mqttConnected = mqttConnect();
    if (mqttConnected) {
      Serial.println("MQTT Connected");
    } else {
      Serial.println("MQTT Connection Failed");
    }
  } else {
    Serial.println("Wi-Fi Connection Failed");
  }
}

void loop() {
  // Loop code can go here
}

bool wiFiConnect() {
  int dotCount = 0;
  uint8_t attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
    delay(RETRY_PERIOD);
    Serial.print(".");
    dotCount++;
    attempts++;

    if (dotCount % DOT_PER_LINE == 0) {
      Serial.println();
    }

    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
  }

  return WiFi.status() == WL_CONNECTED;
}

void wiFiDetails() {
  Serial.println();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());
}

bool mqttConnect() {
  uint8_t attempts = 0;

  while (!mqtt.connected() && attempts < MAX_ATTEMPTS) {
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect()) {
      Serial.println("MQTT Connected!");
      return true;
    } else {
      Serial.println("MQTT Connection Failed, retrying...");
      delay(RETRY_PERIOD);
      attempts++;
    }
  }

  return mqtt.connected();
}
