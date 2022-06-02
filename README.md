# wifi_http_post_from_ttgo_esp32
Shows how to POST to a web endpoint over WiFi using an ESP32 device, the TTGO

Creating a repo on this because it's a little tricky to get working.

Since I'm using a 14 day trial of Zapier starting today, this will fail after that date because you can't
use webhook feature of Zapier without a paid trial.

Used a TTGO T1 Version 1.3

Secrets are in a header file, this also serves to show how to safely hold secrets
That file should have this format:

#define SECRET_SSID "myssid"
#define SECRET_PASS "my password"
#define SECRET_ZAPIER_ENDPOINT_URL "https://hooks.zapier.com/hooks/catch/insert_proper_url_here/"


