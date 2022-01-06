#include <Arduino.h>
#include <esp_sleep.h>
#include "sensors.h"
#include "wifi_helper.h"
#include "config.h"
#include "sleep.h"
#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "OTA_handler.h"
#include <WebSocketsClient.h>
// #include <WebSocketsServer.h>
#include "camera_setup.h"
#include "websocketEvent.h"

// Init instances
DHT dht(PIN_DHT, DHTTYPE);
//WebSocketsClient ws_client;
WebSocketsClient webSocket;
// WebSocketsServer ws_server = WebSocketsServer(81);
unsigned long messageInterval = 5000;
bool connected = false;
bool cam_ready = false;
bool measurements_sent = false;
bool cam_capture_notice_sent = false;
int cam_capture_mod_value = 12;
int cam_capture_wait_time = 60;
int RETRY_MAX = 100;
String measurements = "";
String node_id = NODE_ID;

RTC_DATA_ATTR int bootCount = 0;  //counts number of boots
int loop_count = 0; //counts number of main loops
TempAndHumidity TandH;
float soil_moisture;

void setStaticIP() {
  IPAddress local_IP(192, 168, 1, NODE_IP);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8); // optional
  IPAddress secondaryDNS(8, 8, 4, 4); // optional
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
}

void readSoilMoisture() {
  soil_moisture = get_Soil(reg_b);
  Serial.print("Moisture: ");
  Serial.println(soil_moisture);
  Serial.println("");
}

void readTempHum() {
  char payload[100];
  measurements = "";
  int retries = 0;
  while (1) {
    TandH = get_temp_and_hum();
    if (isnan(TandH.temperature) || isnan(TandH.humidity)) {
      Serial.printf("Could not read Temp/Hum. Retry %d ...\n", retries);
      retries++;
      delay(10);
    } else {
      break;
    }
    if (retries > RETRY_MAX) {
      Serial.println("Could not read Temp/Hum after max retries. Going to sleep now...");
      esp_deep_sleep_start();
    }
  }

  sprintf(payload, "\"temperature\": %0.2f, ", TandH.temperature);
  Serial.print("Temp: ");
  Serial.println(TandH.temperature);
  measurements += payload;

  sprintf(payload, "\"humidity\": %0.2f, ", TandH.humidity);
  Serial.print("Hum: ");
  Serial.println(TandH.humidity);

  measurements += payload;

  sprintf(payload, "\"soil_moisture\": %0.2f", soil_moisture);
  Serial.print("Soil: ");
  Serial.println(soil_moisture);
  measurements += payload;
}

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
    const uint8_t* src = (const uint8_t*) mem;
    Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for(uint32_t i = 0; i < len; i++) {
        if(i % cols == 0) {
            Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        Serial.printf("%02X ", *src);
        src++;
    }
    Serial.printf("\n");
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            Serial.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;

            // send message to server when Connected
            Serial.println("[WSc] SENT: Connected");
            webSocket.sendTXT("Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] RESPONSE: %s\n", payload);
            break;
        case WStype_BIN:
            Serial.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
        case WStype_PING:
            // pong will be send automatically
            Serial.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            Serial.printf("[WSc] get pong\n");
            break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void setup() {
  ready_GPIO();
  Serial.begin(SERIAL_BAUD_RATE);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
      Serial.flush();
      delay(500);
  }

  readSoilMoisture();
  dht.begin();
  readTempHum();

  //setStaticIP();
  InitWiFi();
  LED_on();
  if ((bootCount-1) % cam_capture_mod_value == 0) {
    setup_camera(&cam_ready);
    start_OTA();
  }
  webSocket.begin("68.183.44.212", 12012);
  webSocket.onEvent(webSocketEvent);
}

unsigned long lastUpdate = millis();
unsigned long main_loop_wait = millis();
void loop() {
  webSocket.loop();
  if (cam_ready)
    ArduinoOTA.handle();

  //Send measurements and go back to sleep unless camera is activated
  //Camera activate every 8 cycles
  //When camera is active, give notice to server, loop until timeout, sleep - server should request within that time
  if (connected && lastUpdate+messageInterval<millis()) {
    if (!measurements_sent) {
      Serial.println(measurements);
      webSocket.sendTXT("{\"node_id\": \"" + node_id + "\", " + measurements + "}");
      if (!cam_ready)
        webSocket.sendTXT("{\"node_id\": \"" + node_id + "\", \"cam_ready\":0, \"ip\": " + WiFi.localIP() + "}");
      lastUpdate = millis();
      measurements_sent = true;
      Serial.println("Measurements sent...");
    }

    if (!cam_capture_notice_sent && cam_ready) {
      webSocket.sendTXT("{\"node_id\": \"" + node_id + "\", \"cam_ready\":1, \"ip\": " + WiFi.localIP() + "}");
      cam_capture_notice_sent = true;
      main_loop_wait = millis(); //restart 1 minute wait time
      Serial.println("Camera ready, stay awake longer...");
    }

    if (measurements_sent && !cam_ready) {
      delay(500);
      webSocket.disconnect();
      Serial.println("Done sending measurements, not waiting for camera.");
      Serial.println("Going to Sleep Now...");
      esp_deep_sleep_start(); //Remove for OTA handle
    }
  }

  // Serial.println("Awake...");
  if ((millis() - main_loop_wait) > cam_capture_wait_time*1000) {
    loop_count = 0;
    if (cam_ready) {
      webSocket.sendTXT("{\"node_id\": \"" + node_id + "\", \"cam_ready\":0, \"ip\": " + WiFi.localIP() + "}");
      delay(500);
      webSocket.disconnect();
    }
    Serial.println("Going into battery saving mode: Deep Sleep...");
    esp_deep_sleep_start();
  }
  // delay(100);
  loop_count++;
}
