#include "wifi_helper.h"


int wifi_status = WL_IDLE_STATUS;
int WIFI_RETRY_MAX = 100;
int wifi_retry_count = 0;

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_retry_count++;
    if (wifi_retry_count > WIFI_RETRY_MAX) {
      Serial.println("Could not connect to WiFi. Going to sleep now...");
      esp_deep_sleep_start();
    }
  }
  wifi_retry_count = 0;
  Serial.println("Connected to AP");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  // wifi_status = WiFi.status();
  if ( wifi_status != WL_CONNECTED) {
    InitWiFi();
    // WiFi.begin(WIFI_AP_NAME, WIFI_PASSWORD);
    // while (WiFi.status() != WL_CONNECTED) {
    //   delay(500);
    //   Serial.print(".");
    // }
    // Serial.println("Connected to AP");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());
  }
}


void check_WiFi(){
  // Reconnect to WiFi, if needed
  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
    check_WiFi();
  }
}