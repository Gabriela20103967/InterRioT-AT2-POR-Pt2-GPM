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

#define RETRY_PERIOD 10000 //10 SECONDS
#define DOT_PER_LINE 25

const char* ssid = "WIFI_SSID";
const char* password = "PASSWORD_HERE";

void setup(){
  Serial.begin(9600);
  bool connected = wiFiConnect();
  if (connected) {
    Serial.println("Connected");
    wiFiDetails();
  }else{
    Serial.println("Connection Failed");
  }
}

void loop(){

}

bool wiFiConnect(){
  int dotCount = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(RETRY_PERIOD);

    Serial.print(".");
    dotCount++;

    if (dotCount % DOT_PER_LINE == 0){
      Serial.println();
    }

  }

  return WiFi.status() == WL_CONNECTED;
}



