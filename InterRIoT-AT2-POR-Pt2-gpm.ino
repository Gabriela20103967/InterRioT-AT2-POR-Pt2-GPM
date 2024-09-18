/** 
 * Assessment Title: Portfolio Part 2
 * Cluster:          Intermediate RIoT  
 * Qualification:    ICT50220 Diploma of Information Technology (Advanced Programming) 
 * Name:             Gabriela Morales
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * YOUR SUMMARY OF PORTFOLIO ACTIVITY 
 * GOES HERE 
 * 
 * 
 * Components & Identifiers: 
 * 
 */ 

#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define IO_USERNAME "ADAFRUIT_USERNAME"
#define IO_KEY "ADAFRUIT_KEY"
#define IO_SERVER "ADAFRUIT_SERVER_URI"
#define IO_SERVERPORT 1883

#define RETRY_PERIOD 10000 //10 SECONDS
#define MAX_ATTEMPTS 3
#define DOT_PER_LINE 5

const char* ssid = "WIFI_SSID";
const char* password = "PASSWORD_HERE";

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

void setup(){
  Serial.begin(9600);
  bool connected = wiFiConnect();
  if (connected) {
    Serial.println("Connected");
    wiFiDetails();

    bool mqttConnected = mqttConnect();
    if (mqttConnected){
      Serial.println("MQTT Connected");
    }else{
      Serial.println("MQTT Connection Failed");
    }
  }else{
    Serial.println("Connection Failed");
  }
}

void loop(){

}

bool wiFiConnect(){
  int dotCount = 0;
  uint8_t attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
    Serial.print(".")
    delay(RETRY_PERIOD);
    attempts++;

    dotCount++;
    if (dotCount % DOT_PER_LINE == 0){
      Serial.println();
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

bool mqttConnect(){
  uint8_t attempts = 0;

  while (!mqtt.connected() && attempts < MAX_ATTEMPTS){
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect()) {
      Serial.println("MQTT Connected!");
      return true;
    }else {
      Serial.println("MQTT Connection Failed, retrying...");
      delay(RETRY_PERIOD);
      attempts++;
    }
  }

  return attempts < MAX_ATTEMPTS;
  return false;
}
 




