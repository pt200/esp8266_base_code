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


static const char *TAG="OTA";


/*
curl --data-binary "@simple.bin" -X POST http://192.168.61.251/fw
*/

esp_err_t ota_post_handler(httpd_req_t *req)
{
    char buf[1024];
    int ret, remaining = req->content_len;
    ESP_LOGI(TAG, "content_len: %d", remaining);


    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = NULL;
    ESP_LOGI(TAG, "Starting OTA...");
    update_partition = esp_ota_get_next_update_partition(NULL);
    if (update_partition == NULL) {
        ESP_LOGE(TAG, "Passive OTA partition not found");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype, update_partition->address);

    esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin failed, error=%d", err);
        return err;
    }
    ESP_LOGI(TAG, "esp_ota_begin succeeded");
    ESP_LOGI(TAG, "Please Wait. This may take time");

    esp_err_t ota_write_err = ESP_OK;

    while (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(req, buf,
                        MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                continue;
            }
            return ESP_FAIL;
        }

        if (ret > 0) {
            ota_write_err = esp_ota_write( update_handle, (const void *)buf, ret);
            if (ota_write_err != ESP_OK) {
                break;
            }
//            ESP_LOGI(TAG, "Written image block %d", ret);
        }
        remaining -= ret;
    }

    ESP_LOGI(TAG, "Connection closed,all data received");
    
    esp_err_t ota_end_err = esp_ota_end(update_handle);
    if (ota_write_err != ESP_OK) {
        ESP_LOGE(TAG, "Error: esp_ota_write failed! err=0x%d", err);
        return ota_write_err;
    } else if (ota_end_err != ESP_OK) {
        ESP_LOGE(TAG, "Error: esp_ota_end failed! err=0x%d. Image is invalid", ota_end_err);
        return ota_end_err;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed! err=0x%d", err);
        return err;
    }
    ESP_LOGI(TAG, "esp_ota_set_boot_partition succeeded"); 



    // End response
    httpd_resp_set_status( req, HTTPD_200);
    httpd_resp_send( req, "OK", 2);



//    esp_mqtt_client_disconnect();
//    httpd_stop( req->handle);

    ESP_LOGI(TAG, "Reboot after 5 sec ..."); 
    vTaskDelay( 5000 / portTICK_PERIOD_MS);
    esp_restart();

    return ESP_OK;
}

httpd_uri_t ota_post = {
    .uri       = "/fw",
    .method    = HTTP_POST,
    .handler   = ota_post_handler,
    .user_ctx  = NULL
};


esp_err_t httpd_register_ota_uri_handlers( httpd_handle_t server)
{
    ESP_LOGI(TAG, "OTA Enabled\n    Using:\n      curl --data-binary \"@fw.bin\" -X POST http://[DEVICE IP]/fw");
    return httpd_register_uri_handler( server, &ota_post);
}

