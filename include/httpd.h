#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_err.h>

esp_err_t start_webserver();
esp_err_t stop_webserver();

#ifdef __cplusplus
}
#endif

