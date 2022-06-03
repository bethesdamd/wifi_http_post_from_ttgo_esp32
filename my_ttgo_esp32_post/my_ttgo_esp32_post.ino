/*
* POST some JSON to a Google Sheet row using Zapier as the intermediary
* Project info: https://github.com/bethesdamd/wifi_http_post_from_ttgo_esp32
* Zapier endpoint: https://hooks.zapier.com/hooks/catch/4808229/bab9sly/ 
* Google Sheet: https://docs.google.com/spreadsheets/d/1bRZb_dOTYsurYc7bbOU5AaBCz7FeesKW9asob9E757s/edit#gid=653291478
* Reconnect technique: https://randomnerdtutorials.com/solved-reconnect-esp32-to-wifi/ 
*/


#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>
#include "VariousSecrets.h"
 
const char *AP_SSID = SECRET_SSID;
const char *AP_PWD = SECRET_PASS;
const char *SECRET_ZAPIER_ENDPOINT = SECRET_ZAPIER_ENDPOINT_URL;

WiFiMulti wifiMulti;
 
void setup() {
  Serial.begin(9600);
   
  delay(4000);
  wifiMulti.addAP(AP_SSID, AP_PWD);
}
 
void loop() {
  postDataToServer(5 * 1000 * 60);  // minutes between transmissions
}
 
void postDataToServer(int mSecs) {
 
  Serial.println("Posting JSON data to server...");
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
     
    HTTPClient http;   
     
    http.begin(SECRET_ZAPIER_ENDPOINT);  
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<200> doc;
    // Add values in the document
    // This is the payload.  Each item below maps to a column in the spreadsheet.
    // So if you have a spreadsheet with a column called "timestamp", then you 
    // must set doc["timestamp"] = "03/24/1988" for example.
    doc["name"] = "david-6";
    doc["location"] = String(random(105));  
   
    // Add an array
    // JsonArray data = doc.createNestedArray("data");
    // data.add(48.756080);
    // data.add(2.302038);
     
    String requestBody;
    serializeJson(doc, requestBody);
     
    int httpResponseCode = http.POST(requestBody);
 
    if(httpResponseCode>0){
       
      String response = http.getString();                       
       
      Serial.println(httpResponseCode);   
      Serial.println(response);
     
    }
    else {
     
      //Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
      Serial.printf("Error occurred while sending HTTP POST\n");
       
    }
    delay(mSecs);
     
  }
}