// http_upload.h
#pragma once
#include "esp_err.h"
#include "esp_camera.h"

esp_err_t send_image(camera_fb_t *fb);
