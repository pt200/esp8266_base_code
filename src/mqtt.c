/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "my_app.h"

static const char *TAG = "MQTT_MAIN";



static esp_mqtt_client_handle_t mqtt_client;


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_user_handler_cb(event_data);
}

int mqtt_publish_data( char* TAG_name, char* data)
{
    char topic_name[ 256];
    sprintf( topic_name, CONFIG_MQTT_DEVICE_DEFAULT_PATH "/%s", TAG_name);
printf( "MQTT_PUBLISH: %s = %s\n", topic_name, data);
    return esp_mqtt_client_publish( mqtt_client, topic_name, data, 0, APP_MQTT_QOS, 0);
}

int mqtt_publish_error( char* err_text)
{
    return mqtt_publish_data( "error", err_text);
}

int mqtt_publish_data_full_path( char* topic_name, char* data)
{
printf( "MQTT_PUBLISH: %s = %s\n", topic_name, data);
    return esp_mqtt_client_publish( mqtt_client, topic_name, data, 0, APP_MQTT_QOS, 0);
}



esp_err_t mqtt_client_start()
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = CONFIG_MQTT_BROKER_URL, // "mqtt://mqtt.iot"
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event( mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);

    esp_mqtt_client_start( mqtt_client);
    return ESP_OK;
}

