// http_upload.c
#include "http_upload.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "wifi_module.h"

#define SERVER_URL "http://192.168.33.93:8000/upload"

static const char *TAG = "HTTP_UPLOAD";

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    if (evt->event_id == HTTP_EVENT_ON_DATA) {
        ESP_LOGI(TAG, "Received response from server:");
        printf("%.*s\n", evt->data_len, (char *)evt->data);
    }
    return ESP_OK;
}

esp_err_t send_image(camera_fb_t *fb) {
    ESP_LOGI(TAG, "Sending image to server...");

    esp_http_client_config_t config = {
        .url = SERVER_URL,
        .event_handler = _http_event_handler,
        .disable_auto_redirect = true,
        .timeout_ms = 5000,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Failed to init http client");
        return ESP_FAIL;
    }

    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_post_field(client, (const char *)fb->buf, fb->len);
    esp_http_client_set_header(client, "Content-Type", "image/jpeg");

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Image sent successfully, HTTP status = %d",
                 esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "Failed to send image: %s", esp_err_to_name(err));
        // wifi_reconnect();  // 网络出错时尝试重连 WiFi
    }

    esp_http_client_cleanup(client);
    return err;
}

