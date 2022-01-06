//Software
#define FIRMWARE_VERSION "0.0.1"

// Constants
#define PIN_DHT 15 // Digital pin connected to the DHT sensor
#define PIN_SOIL 14 // Analog pin for soil sensor
#define PIN_BATTERY 12 // Analog pin for battery level
#define PIN_LED 33 // Pin for onboard led
#define PIN_FLASH 4 // Pin for builtin flash
#define DHTTYPE DHT22 //DHT22 sensor
#define SERIAL_BAUD_RATE 115200 // Serial speed

// Sleep
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  600      /* Time ESP32 will go to sleep (in seconds) */

// WiFi
#define WIFI_AP_NAME       "Plaas_Tonnels" //"Gumball" //"wlan-ap" //Wifi ssid
#define WIFI_PASSWORD      "Plaas123!@#" //"Jaggles1!" //0004edb3faee"  // WiFi password
//#define WIFI_AP_NAME       "Sonic" //"Gumball" //"wlan-ap" //Wifi ssid
//#define WIFI_PASSWORD      "Jaggles1!" //0004edb3faee"  // WiFi password
// #define WIFI_AP_NAME       "Krause WiFi_EXT" //"Gumball" //"wlan-ap" //Wifi ssid
// #define WIFI_PASSWORD      "Heide@60" //0004edb3faee"  // WiFi password

#define NODE_ID "ESP32_004"
#define NODE_IP 102