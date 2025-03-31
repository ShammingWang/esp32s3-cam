#pragma once

#include "esp_err.h"
#include "esp_camera.h"

esp_err_t camera_module_init();
camera_fb_t *camera_module_capture();
void camera_module_return(camera_fb_t *fb);
