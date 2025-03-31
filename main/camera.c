#include "camera.h"
#include "esp_log.h"

static const char *TAG = "CAM_MOD";

esp_err_t camera_module_init() {
    camera_config_t camera_config = {
        .pin_pwdn = -1,
        .pin_reset = -1,
        .pin_xclk = 15,
        .pin_sccb_sda = 4,
        .pin_sccb_scl = 5,
        .pin_d0 = 11,
        .pin_d1 = 9,
        .pin_d2 = 8,
        .pin_d3 = 10,
        .pin_d4 = 12,
        .pin_d5 = 18,
        .pin_d6 = 17,
        .pin_d7 = 16,
        .pin_vsync = 6,
        .pin_href = 7,
        .pin_pclk = 13,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_SVGA,  //
        .jpeg_quality = 10, //
        .fb_count = 2,  // tow frames for cache
        .fb_location = CAMERA_FB_IN_PSRAM, // use the external psram
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera Init Failed: %s", esp_err_to_name(err));
    }
    return err;
}

camera_fb_t *camera_module_capture() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(TAG, "Camera capture failed!");
    }
    return fb;
}

void camera_module_return(camera_fb_t *fb) {
    if (fb) esp_camera_fb_return(fb);
}
