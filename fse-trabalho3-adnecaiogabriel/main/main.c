#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "dht11.h"
#include "LED.h"
#include "hall.h"
#include "mosquitto.h"
#include "wifi.h"
#include "mqtt.h"
#include "mosquitto.h"
#include "sound_sensor.h"
#include "json_parser.h"
#include "buzzer.h"
#include "gpio_setup.h"
#include <math.h>
#include "global.h"
#include "light_sleep.h"
#include "nvs.h"
#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER
#define ESP_MODE CONFIG_ESP_MODE
#define BATTERY_MODE 0
#define ENERGY_MODE 1
#include "security.h"
#include "simple_ota_example.h"
SemaphoreHandle_t connectionWifiSemaphore;
SemaphoreHandle_t connectionMQTTSemaphore;
SemaphoreHandle_t reconnectionWifiSemaphore;
#include "security.h"


float temp_media = 0;
float humidity_media = 0;
int cont_temp = 0;

void wifi_connected(void * params)
{
  while(true)
  {
    if(xSemaphoreTake(connectionWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
      mosquitto_start();
    }
  }
}

void handle_server_communication(void)
{
  char mensagem[50];
  if(xSemaphoreTake(connectionMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       int32_t ALARME=le_valor_nvs("alarme");
       sprintf(mensagem, "{\"alarme\": %ld}", ALARME);
       mqtt_envia_mensagem("v1/devices/me/attributes", mensagem);
       vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }
}

float limit_decimal(float x, int decimal_places){
  float power = pow(10, decimal_places);
  return roundf(x*power)/power;
}

void app_main(void)
{    
    // if(MODO_OTA==1){
    //   simple_ota_example();
    // }
    if(ESP_CONFIG_NUMBER==1){
      int32_t ALARME=le_valor_nvs("alarme");
      char *tag = le_string_nvs("tag");
      
      if(ALARME==1){
        xTaskCreate(&security, "Ativar segurança", 4096, (void*)tag, 1, NULL);
      }
    }
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    connectionWifiSemaphore = xSemaphoreCreateBinary();
    connectionMQTTSemaphore = xSemaphoreCreateBinary();
    reconnectionWifiSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&wifi_connected,  "Conexão ao MQTT", 4096, NULL, 1, NULL);

    configure_ESP_BUTTON();

    if(ESP_MODE == BATTERY_MODE) {
        if(ESP_CONFIG_NUMBER == 0) {
        wake_up_with_hall();
        configure_HALL();
        printf("aqui");
          while(1){
            ESP_LOGI("Modo Funcionamento", "Bateria");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            light_sleep_task();
          }
    } else {
        printf("ESP not identified");
    }
    }
    else if(ESP_MODE == ENERGY_MODE) {
      ESP_LOGI("Modo Funcionamento", "ENERGIA");
      
      if(ESP_CONFIG_NUMBER == 0) {
        DHT11_init(4);
        xTaskCreate(&verifica_magnetic, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
        xTaskCreate(&check_temperature, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
      } else if(ESP_CONFIG_NUMBER == 1) {

      } else if(ESP_CONFIG_NUMBER == 2) {
        configure_LED();
        configure_SOUND();
        xTaskCreate(&check_sound, "Leitura Sensor de Som", 4096, NULL, 1, NULL);
      } else {
        printf("ESP not identified");
      }
    }
}