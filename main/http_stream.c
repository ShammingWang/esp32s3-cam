#include "esp_log.h"
#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_netif.h"  // 需要包含它来获取 IP 地址


static const char *TAG = "HTTP_STREAM";

esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t *fb = NULL;
    char *part_buf[64];

    httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");

    while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            continue;
        }

        // 写入MJPEG帧头
        int hlen = snprintf((char *)part_buf, sizeof(part_buf),
            "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
        httpd_resp_send_chunk(req, (const char *)part_buf, hlen);

        // 写入图像数据
        httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        httpd_resp_send_chunk(req, "\r\n", 2);

        esp_camera_fb_return(fb);

        vTaskDelay(pdMS_TO_TICKS(100)); // 控制帧率（10 FPS）
    }

    return ESP_OK;
}

esp_err_t start_stream_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start HTTP server");
        return ESP_FAIL;
    }

    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL
    };
    httpd_register_uri_handler(server, &stream_uri);

    // 👇 获取当前 IP 地址
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        ESP_LOGI(TAG, "Stream server started on http://" IPSTR "/stream", IP2STR(&ip_info.ip));
    } else {
        ESP_LOGW(TAG, "Failed to get IP address");
    }

    return ESP_OK;
}


