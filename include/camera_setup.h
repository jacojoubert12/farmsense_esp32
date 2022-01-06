#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include "wifi_helper.h"


void startCameraServer();
void setup_camera(bool *cam_ready);