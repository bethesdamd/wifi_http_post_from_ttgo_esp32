# wifi_http_post_from_ttgo_esp32
Shows how to POST to a web endpoint over WiFi using an ESP32 device, the TTGO.  Specifically this is posting some json to a Make.com webhook endpoint,
which is configure to create a new row in a Google Sheet.  You can't easily post directly to Google Sheets, so Make.com makes it easier.

*NOTE: This will not work on a "captive" WiFi network.  Captive networks require entering information into a web page.*

This code is very basic and will not re-try to get a connection if the connection drops.

This demonstrates a very useful pattern:  how to send information from a sensor or other device, wirelessly to a Google spreadsheet.  Each transmission
has one or more columns and becomes a row in the spreadsheet.  Once you have data in the sheet, you can view, analyze, chart, etc.  This is 
great for classroom science and other projects because you don't have to do anything to get data from a device like a thermometer,
air quality sensor, light sensor, etc.

Creating a repo on this because it's a little tricky to get working.

Used a TTGO T1 Version 1.3:  https://github.com/LilyGO/ESP32-TTGO-T1 

Secrets are in a header file, this also serves to show how to safely hold secrets
That file should have this format:

#define SECRET_SSID "myssid"
#define SECRET_PASS "my password"
#define SECRET_ENDPOINT_URL "https://hook.integromat.com/<YOUR ID GOES HERE>"


