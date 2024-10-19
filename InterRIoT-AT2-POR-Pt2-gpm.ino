
 /**
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * This code is trying to publish the data into my feed in my adafruit account
 * 
 */ 

#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define IO_USERNAME "ADAFRUIT_NAME"
#define IO_KEY "ADAFRUIT_KEY"
#define IO_SERVER "ADAFRUIT_SERVER_URI"
#define IO_SERVERPORT 1883
#define IO_FEED "inter-riot-at2-data"

const char* ssid = "WIFI_HERE";
const char* password = "PASSWORD_HERE";

#define RETRY_PERIOD 500
#define RETRY_ADJUSTMENT 125
#define MAX_ATTEMPTS 5
#define photoresistorPin 32

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Publish lightFeed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" IO_FEED);

int lightValue = 0;

void setup() {
  Serial.begin(9600);

  bool wifiConnected = wiFiConnect();
  if (wifiConnected) {
    Serial.println("Wi-Fi Connected");
    wiFiDetails();

    bool mqttConnected = mqttConnect();
    if (mqttConnected) {
      Serial.println("MQTT Connected");
    } else {
      Serial.println("MQTT Connection Failed.");
    }
  } else {
    Serial.println("Wi-Fi Connection Failed");
  }
}

void loop() {
  lightValue = analogRead(photoresistorPin);
  Serial.print("Light Value: ");
  Serial.println(lightValue);
  if (!lightFeed.publish((int32_t)lightValue)) {
    Serial.println("Failed to publish data");
  } else {
    Serial.println("data published successfully");
  }
  delay(5000); 
}

bool wiFiConnect() {
  uint8_t attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
    Serial.print(".");
    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;
  }

  return WiFi.status() == WL_CONNECTED;
}

bool mqttConnect() {
  uint8_t attempts = 0;
  int8_t mqttConnectionResult;

  Serial.println("Connecting to MQTT...");
  
  while ((mqttConnectionResult = mqtt.connect()) != 0 && attempts < MAX_ATTEMPTS) {
    Serial.print(".");
    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;

    if (mqttConnectionResult != 0) {
      mqtt.disconnect();
    }
  }

  return mqtt.connected();
}

void wiFiDetails() {
  Serial.println();
  Serial.print("Local IP:        ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway IP:      ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());
  Serial.print("Hostname:        ");
  Serial.println(WiFi.getHostname());
}