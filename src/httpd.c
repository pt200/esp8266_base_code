/* Simple HTTP Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"

#include <esp_http_server.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_https_ota.h>
#include <esp_ota_ops.h>
#include "sdkconfig.h"

#include "ota.h"
#include "httpd.h"
#include "my_app.h"

static const char *TAG="HTTPD";
static httpd_handle_t server = NULL;

esp_err_t start_webserver()
{
    if (server == NULL) {
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();

        // Start the httpd server
        ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
        if (httpd_start(&server, &config) == ESP_OK) {
            // Set URI handlers
            ESP_LOGI(TAG, "Registering URI handlers");
            httpd_register_ota_uri_handlers( server);
//#ifdef HTTPD_USER_HANDLERS
            httpd_register_user_uri_handlers( server);
//#endif
            return ESP_OK;
        }

        ESP_LOGI(TAG, "Error starting server!");
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t stop_webserver()
{
    if( server) {
        httpd_stop( server);
        server = NULL; // Global
    }
    return ESP_OK;
}

