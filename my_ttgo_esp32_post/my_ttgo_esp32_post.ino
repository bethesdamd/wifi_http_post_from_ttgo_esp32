/*
* POST some JSON to a Google Sheet row using Zapier as the intermediary
* Project info: https://github.com/bethesdamd/wifi_http_post_from_ttgo_esp32
* Zapier endpoint: https://hooks.zapier.com/hooks/catch/4808229/bab9sly/ 
* Integromat / Make.com endpoint: https://hook.integromat.com/d3tpfvvj33ein31v7gohun58l9qvwt4a
* Google Sheet: https://docs.google.com/spreadsheets/d/1bRZb_dOTYsurYc7bbOU5AaBCz7FeesKW9asob9E757s/edit#gid=653291478
* Reconnect technique: https://randomnerdtutorials.com/solved-reconnect-esp32-to-wifi/ 
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
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  char timeStringBuff[50];

  // "A": Saturday
  // %d/%e/%Y %H:%M:%S
  // strftime(timeStringBuff, sizeof(timeStringBuff), "%B %d %Y %H:%M:%S", &timeinfo);
  strftime(timeStringBuff, sizeof(timeStringBuff), "%D %H:%M:%S", &timeinfo);
  String mystring;
  mystring = timeStringBuff;
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
  postDataToServer(3 * 1000 * 60);  // minutes between transmissions
}
 
void postDataToServer(int mSecs) {
  Serial.println("=========================================================\n");
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {

  //init and get the time
  configTime(gmtOffset_sec * -5, daylightOffset_sec, ntpServer);
  String datetime;
  datetime = returnLocalTime();
  Serial.println(datetime);
  Serial.println("\n");
     
  HTTPClient http;   
    
  http.begin(SECRET_ENDPOINT);  
  http.addHeader("Content-Type", "application/json");         
    

    
  StaticJsonDocument<200> doc;
  // Add values in the document
  // This is the payload.  Each item below maps to a column in the spreadsheet.
  // So if you have a spreadsheet with a column called "timestamp", then you 
  // must set doc["timestamp"] = "03/24/1988" for example.

  doc[String("timestamp")] = datetime;
  // Serial.println("test datetime print:");
  // Serial.println(doc["timestamp"]);
 
  sensorValue = analogRead(sensorPin);
  Serial.println("Sensor value:");
  Serial.println(sensorValue);
  doc["value"] = String(sensorValue);
     
  String requestBody;
  // String input = "{\"timestamp\":\"mytime\",\"value\":12345}";
  // deserializeJson(doc, input);
  serializeJson(doc, requestBody);
  
  int httpResponseCode = http.POST(requestBody);

  if(httpResponseCode>0){
      
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
}