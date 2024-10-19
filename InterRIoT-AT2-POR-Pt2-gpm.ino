/**
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * This code is trying to connect to wifi, and if successful, will turn on a led,
 * and also tries to send data to the MQTT broker, with one led flashing when the data is sent.
 * 
 * Components:
 * ESP32 Wrover
 * LED [RED]        LED_RED_1
 * LED [BLUE]       LED_BLUE_1
 * Photoresistor
 * Resistor [10Ω]
 * Resistor [220Ω]
 * Resistor [220Ω]
 */

#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define IO_USERNAME "ADAFRUIT_NAME"
#define IO_KEY "ADAFRUIT_KEY"
#define IO_SERVER "io.adafruit.com"
#define IO_SERVERPORT 1883
#define IO_FEED "inter-riot-at2-data"

#define RETRY_PERIOD 500
#define RETRY_ADJUSTMENT 125
#define MAX_ATTEMPTS 5

#define LED_BLUE_1 13
#define LED_RED_1 12
#define PHOTORESISTOR_PIN 32  

const char* ssid = "WIFI_SSID";         
const char* password = "PASSWORD_HERE";

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Publish lightFeed = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/" IO_FEED);
int lightValue = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);
  
  bool wifiConnected = wiFiConnect();
  if (wifiConnected) {
    Serial.println("Wi-Fi Connected");
    digitalWrite(LED_BLUE_1, HIGH);  

    bool mqttConnected = mqttConnect();
    if (!mqttConnected) {
      Serial.println("MQTT Connection Failed.");
    } else {
      Serial.println("MQTT Connected");
    }
  } else {
    Serial.println("Wi-Fi Connection Failed");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && mqtt.connected()) {
    lightValue = analogRead(PHOTORESISTOR_PIN);
    Serial.print("Value: ");
    Serial.println(lightValue);

    if (!lightFeed.publish((int32_t)lightValue)) {
      Serial.println("Failed to publish data");
    } else {
      Serial.println("Data published successfully");
      digitalWrite(LED_RED_1, HIGH);
      delay(100);
      digitalWrite(LED_RED_1, LOW);
    }
    
    delay(5000);  
  } else {
    Serial.println("Wi-Fi or MQTT not connected. Cannot publish data.");
  }
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

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi!");
  } else {
    Serial.println("Failed to connect to Wi-Fi.");
  }

  return WiFi.status() == WL_CONNECTED;  
}

bool mqttConnect() {
  uint8_t attempts = 0;
  int8_t mqttConnection;

  Serial.println("Connecting to MQTT...");

  while ((mqttConnection = mqtt.connect()) != 0 && attempts < MAX_ATTEMPTS) {
    Serial.print(".");
    delay(RETRY_PERIOD + RETRY_ADJUSTMENT * attempts);
    attempts++;

    if (mqttConnection != 0) {
      Serial.print("MQTT Connection failed with error: ");
      Serial.println(mqttConnectionResult);
      mqtt.disconnect();   
    }
  }

  if (mqtt.connected()) {
    Serial.println("Connected to MQTT!");
  } else {
    Serial.println("Failed to connect to MQTT.");
  }

  return mqtt.connected();   
}
