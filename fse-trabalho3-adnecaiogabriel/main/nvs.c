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
#include "esp_log.h"

#define STORAGE_NAMESPACE "storage"


esp_err_t save_int(int32_t data, char* variable_name) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open
    err= nvs_open_from_partition("DadosNVS", STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
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
    err= nvs_open_from_partition("DadosNVS", STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
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




int32_t le_valor_nvs(char *variable)
{
    // Inicia o acesso à partição padrão nvs
    ESP_ERROR_CHECK(nvs_flash_init());

    // Inicia o acesso à partição personalizada
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    int32_t valor = 0;
    nvs_handle particao_padrao_handle;
    
    // Abre o acesso à partição nvs
    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);
    
    // Abre o acesso à partição DadosNVS
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READONLY, &particao_padrao_handle);
    


    if(res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    else
    {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, variable, &valor);

        switch (res)
        {
        case ESP_OK:
            ESP_LOGI("NVS", "Valor armazenado: %d\n", (int) valor);
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE("NVS", "Valor não encontrado");
            return -1;
        default:
            ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
            return -1;
            break;
        }

        nvs_close(particao_padrao_handle);
    }
    return valor;
}

void grava_valor_nvs(int32_t valor,char *variable)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
    // esp_err_t res_nvs = nvs_open_from_partition("DadosNVS", "armazenamento", NVS_READWRITE, &particao_padrao_handle);
    
    if(res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    esp_err_t res = nvs_set_i32(particao_padrao_handle, variable, valor);
    if(res != ESP_OK)
    {
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    }
    nvs_commit(particao_padrao_handle);
    nvs_close(particao_padrao_handle);
}
