#include <inttypes.h>
#include "nvs_flash.h"

#ifndef NVS_H
#define NVS_H

esp_err_t save_int(int32_t data, char* variable_name);
int32_t read_int(char* variable_name);
esp_err_t write_str(char* key, char* stringVal);
char* read_str(char* key);
int32_t le_valor_nvs(char *variable);
void grava_valor_nvs(int32_t valor,char *variable);
char* le_string_nvs(char *variable);
void grava_string_nvs(char* valor,char *variable);

#endif