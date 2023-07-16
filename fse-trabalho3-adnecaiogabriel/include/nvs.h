#include <inttypes.h>
#include "nvs_flash.h"

#ifndef NVS_H
#define NVS_H

esp_err_t save_int(int32_t data, char* variable_name);
int32_t read_int(char* variable_name);
esp_err_t write_str(char* key, char* stringVal);
char* read_str(char* key);

#endif