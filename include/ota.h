#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>
#include <esp_http_server.h>

esp_err_t httpd_register_ota_uri_handlers( httpd_handle_t server);

#ifdef __cplusplus
}
#endif

