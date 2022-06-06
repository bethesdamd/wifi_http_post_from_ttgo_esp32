/*
* POST sensor data to a Google Sheet row using Make.com (formerly Integromat) as the intermediary
* Project info: https://github.com/bethesdamd/wifi_http_post_from_ttgo_esp32
* Google Sheet: https://docs.google.com/spreadsheets/d/1bRZb_dOTYsurYc7bbOU5AaBCz7FeesKW9asob9E757s/edit#gid=653291478
* Reconnect technique: https://randomnerdtutorials.com/solved-reconnect-esp32-to-wifi/ 

Uses NTP to get date time since this microcontroller lacks a realtime clock
*
* Reads analog voltage value from pin 35 of TTGO.  Grove analog light sensor produces
* values from approx 650 for dark to 2200 and above for bright cloudy day
* Press T1 RST button to re-run code
*/

#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
#include "VariousSecrets.h"
#include "time.h"
 
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

const char *AP_SSID = SECRET_SSID;
const char *AP_PWD = SECRET_PASS;
const char *SECRET_ENDPOINT = SECRET_ENDPOINT_URL;

WiFiMulti wifiMulti;  // wifiMulti is able to pick best of multiple AP's but this code isn't using that feature
 
int sensorValue = 0;
int sensorPin = 35;

String returnLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "xxx";
  }
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%D %H:%M:%S", &timeinfo);
  String mystring;
  mystring = timeStringBuff;
  Serial.println("mystring: " + mystring);
  return mystring;
}

void setup() {
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
  Serial.println("Hello from setup");
}
 
void loop() {
  postDataToServer(1 * 1000 * 60);  // minutes between transmissions
}
 
void postDataToServer(int mSecs) {
  Serial.println("=========================================================\n");
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {

  configTime(gmtOffset_sec * -5, daylightOffset_sec, ntpServer);
  String datetime;
  datetime = returnLocalTime();
  Serial.println(datetime);
  Serial.println("\n");
     
  HTTPClient http;   
    
  http.begin(SECRET_ENDPOINT);  
  http.addHeader("Content-Type", "application/json");         
    
  String requestBody;

  StaticJsonDocument<200> jsonDoc;
  JsonObject stuff = jsonDoc.createNestedObject("stuff");
  stuff["timestamp"] = returnLocalTime();
  stuff["value"] = analogRead(sensorPin);
  char out[150];
  serializeJson(jsonDoc, out);
  int httpResponseCode = http.POST(out);
  String response = http.getString();                       
  Serial.println("HTTP response code:");
  Serial.println(httpResponseCode);  
  Serial.println("HTTP response"); 
  Serial.println(response);

  }
  else {
    //Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
    Serial.printf("Error occurred while sending HTTP POST\n");
  }
  delay(mSecs);

}