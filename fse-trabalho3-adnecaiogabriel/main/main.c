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

#include "wifi.h"
#include "mqtt.h"
#include "mosquitto.h"
#include "sound_sensor.h"
#include "json_parser.h"
#include "gpio_setup.h"
#include <math.h>
#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER

#define ESP_MODE CONFIG_ESP_MODE
#define BATTERY_MODE 0
#define ENERGY_MODE 1

SemaphoreHandle_t connectionWifiSemaphore;
SemaphoreHandle_t connectionMQTTSemaphore;
SemaphoreHandle_t reconnectionWifiSemaphore;

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

void handle_server_communication(void * params)
{
  char mensagem[50];
  char jsonAtributos[200];
  if(xSemaphoreTake(connectionMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       float temp = 20.0 + (float)rand()/(float)(RAND_MAX/10.0);
       
    //    sprintf(mensagem, "{\"temperature\": %f}", temp);
    //    mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);

    //    sprintf(jsonAtributos, "{\"quantidade de pinos\": 5, \n\"umidade\": 20}");
    //    mqtt_envia_mensagem("v1/devices/me/attributes", jsonAtributos);

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
    xTaskCreate(&handle_server_communication, "Comunicação com Broker", 4096, NULL, 1, NULL);

    if(ESP_MODE == BATTERY_MODE) {
      if(ESP_CONFIG_NUMBER == 2) {
        //wake_up_with_gpio(16);
        while(1) {
          vTaskDelay(1000 / portTICK_PERIOD_MS);
          ESP_LOGI("Modo Funcionamento", "Bateria");
        //   light_sleep_task();
          xTaskCreate(&wifi_connected,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
          xTaskCreate(&handle_server_communication, "Comunicação com Broker", 4096, NULL, 1, NULL);
        }
      } else {
        printf("ESP not identified");
      }

    }
    else if(ESP_MODE == ENERGY_MODE) {
      ESP_LOGI("Modo Funcionamento", "ENERGIA");
      
      if(ESP_CONFIG_NUMBER == 0) {
        DHT11_init(4);
        configure_HALL();
        xTaskCreate(&verifica_magnetic, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
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



