/** 
 * Assessment Title: Portfolio Part 2
 * Cluster:          Intermediate RIoT  
 * Qualification:    ICT50220 Diploma of Information Technology (Advanced Programming) 
 * Name:             Gabriela Morales
 * Student ID:       20103967
 * Year/Semester:    2024/S2 
 * 
 * This code is trying to connect to wifi and mqtt while is awake, and after that it will stop for 30 minutes and try to connect again,
 * without passing the max attempts.
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

#define uS_TO_S_FACTOR 1000000 // microseconds per second
#define TIME_TO_SLEEP 1800        // 30 minutes
#define RETRY_PERIOD 10000      // 10 seconds
#define MAX_WIFI_ATTEMPTS 6     
#define MAX_MQTT_ATTEMPTS 10    
#define DOT_PER_LINE 5

const char* ssid = "WIFI_SSID";         
const char* password = "PASSWORD_HERE"; 

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

RTC_DATA_ATTR int bootCount = 0; 

void setup() {
  Serial.begin(115200);
  delay(1000);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();

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

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Set up ESP32 to sleep every " + String(TIME_TO_SLEEP) + " seconds");

  Serial.println("Going to sleep in 1 second...");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here
}

bool wiFiConnect() {
  int dotCount = 0;
  uint8_t attempts = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_ATTEMPTS) {
    delay(RETRY_PERIOD);
    Serial.print(".");
    dotCount++;
    attempts++;

    Serial.print("Wi-Fi Attempt: ");
    Serial.print(attempts);
    Serial.print(" - Wi-Fi Status: ");
    Serial.println(WiFi.status());

    if (dotCount % DOT_PER_LINE == 0) {
      Serial.println();
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi connection established!");
  } else {
    Serial.println("Failed to connect to Wi-Fi.");
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

  while (!mqtt.connected() && attempts < MAX_MQTT_ATTEMPTS) {
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect()) {
      Serial.println("MQTT Connected!");
      return true;
    } else {
      Serial.println("MQTT Connection Failed, retrying...");
      delay(RETRY_PERIOD);
      attempts++;

      Serial.print("MQTT Attempt: ");
      Serial.println(attempts);
    }
  }

  return mqtt.connected();
}

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

}
