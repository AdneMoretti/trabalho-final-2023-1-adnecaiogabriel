/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"


esp_err_t save_int(int32_t data, char* variable_name) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    // Write
    err = nvs_set_i32(my_handle, variable_name, data);
    if (err != ESP_OK) return err;

    // Commit written value.
    // After setting any values, nvs_commit() must be called to ensure changes are written
    // to flash storage. Implementations may write to storage at other times,
    // but this is not guaranteed.
    err = nvs_commit(my_handle);
    if (err != ESP_OK) return err;

    // Close
    nvs_close(my_handle);
    return ESP_OK;
}

int32_t read_int(char* variable_name) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    // Read
    int32_t data = 0; // value will default to 0, if not set yet in NVS
    err = nvs_get_i32(my_handle, variable_name, &data);
    if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return 0;

    nvs_close(my_handle);
    return data;
}

esp_err_t write_str(char* key, char* stringVal) {
    nvs_handle_t nvsHandle;
    esp_err_t retVal;
    
    retVal = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if (retVal != ESP_OK) return retVal;
    
    retVal = nvs_set_str(nvsHandle, key, stringVal);
    if (retVal != ESP_OK) return retVal;

    retVal = nvs_commit(nvsHandle);
    if (retVal != ESP_OK) return retVal;
    
    nvs_close(nvsHandle);

    return ESP_OK;
}

char* read_str(char* key) {
    nvs_handle_t nvsHandle;
    esp_err_t retVal;
    
    retVal = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &nvsHandle);
    if (retVal != ESP_OK) return "";

    size_t required_size;
    retVal = nvs_get_str(nvsHandle, key, NULL, &required_size);
    if (retVal != ESP_OK) return "";
    
    char* savedData = malloc(required_size);
    retVal = nvs_get_str(nvsHandle, key, savedData, &required_size);
    if (retVal != ESP_OK) return "";
    
    nvs_close(nvsHandle);
    
    return savedData;
}