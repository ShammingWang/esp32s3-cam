idf_component_register(
    SRCS "main_app.c" 
    "camera_module.c" 
    "http_upload.c" 
    "wifi_module.c" 
    "http_stream.c"
    INCLUDE_DIRS "."
    REQUIRES esp32-camera esp_wifi esp_event esp_http_client nvs_flash esp_http_server
)
