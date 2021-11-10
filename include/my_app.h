#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>
#include "esp_http_server.h"
#include "mqtt_client.h"

esp_err_t user_app_main();

/*
esp_err_t ota_post_handler(httpd_req_t *req)
{
    httpd_resp_set_status( req, HTTPD_200);
    httpd_resp_send( req, "OK", 2);

    return ESP_OK;
}

httpd_uri_t test_handler = {
    .uri       = "/test",
    .method    = HTTP_TEST,
    .handler   = test_handler,
    .user_ctx  = NULL
};

esp_err_t httpd_register_ota_uri_handlers( httpd_handle_t server)
{
    return httpd_register_uri_handler( server, &test_handler);
}

*/
esp_err_t httpd_register_user_uri_handlers( httpd_handle_t server);

/*
esp_err_t mqtt_event_user_handler_cb( esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
//            printf("MQTT_EVENT_DATA: %.*s = %.*s\n", event->topic_len, event->topic, event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}
*/
esp_err_t mqtt_event_user_handler_cb( esp_mqtt_event_handle_t event);

#ifdef __cplusplus
}
#endif

