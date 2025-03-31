#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "camera_module.h"
#include "wifi_module.h"
#include "http_upload.h"

#include "http_stream.h"  // 别忘了包含


static const char *TAG = "CAM_HTTP_APP";

void app_main(void) {
    ESP_LOGI(TAG, "App starting...");
    ESP_ERROR_CHECK(nvs_flash_init());

    wifi_init_sta();

    ESP_LOGI(TAG, "Initializing camera...");
    if (camera_module_init() != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed");
        return;
    }

    // start_stream_server();  // ← 加上这个！

    // while(1) {
    //     ESP_LOGI("APP", "Doing other tasks...");
    //     vTaskDelay(pdMS_TO_TICKS(5000));
    // }
    while(1) {
        ESP_LOGI(TAG, "10seconds print");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }

    while (1) {
        ESP_LOGI(TAG, "Capturing image...");
        camera_fb_t *fb = camera_module_capture();
        if (fb) {
            ESP_LOGI(TAG, "Image size: %d bytes", fb->len);
            send_image(fb);
            camera_module_return(fb);
        } else {
            ESP_LOGW(TAG, "Capture failed");
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
