/** 
 * Assessment Title: Portfolio Part 2
 * Cluster:          Intermediate RIoT  
 * Qualification:    ICT50220 Diploma of Information Technology (Advanced Programming) 
 * Name:             Gabriela Morales
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * This code is trying to coonect to wifi and if is successful will turn on a led, and also trying to sent data
 * to the MQTT broker and one led will be flash when the data is sent.
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

#define IO_USERNAME "ADAFRUIT_USERNAME"
#define IO_KEY "ADAFRUIT_KEY"
#define IO_SERVER "io.adafruit.com"
#define IO_SERVERPORT 1883
#define IO_FEED "inter-riot-at2-data"

#define RETRY_PERIOD 500
#define RETRY_ADJUSTMENT 125
#define MAX_ATTEMPTS 5

#define LED_BLUE_1 37  
#define LED_RED_1 43  
#define photoresistorPin 32  

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
  Serial.print("Value: ");
  Serial.println(lightValue);
  
  if (!lightFeed.publish((int32_t)lightValue)) {
    Serial.println("Failed to publish data");
  } else {
    Serial.println("data published successfully");
    
    digitalWrite(LED_RED_1, HIGH);
    delay(100);
    digitalWrite(LED_RED_1, LOW);
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