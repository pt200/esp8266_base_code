#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>

#define APP_MQTT_QOS    ( 0)
          

int mqtt_publish_data( char* TAG_name, char* data);
int mqtt_publish_error( char* err_text);
int mqtt_publish_data_full_path( char* topic_name, char* data);
esp_err_t mqtt_client_start();

#ifdef __cplusplus
}
#endif

