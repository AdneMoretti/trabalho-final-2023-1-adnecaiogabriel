#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "dht11.h"
#include "hall.h"
#include "mosquitto.h"
#include "wifi.h"
#include "mqtt.h"
#include "gpio_setup.h"
#include <math.h>
#include "global.h"
#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER
#include "nvs.h"
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
      mosquitto_start();
      mqtt_start();
    }
  }
}

void handle_server_communication(void)
{
  char mensagem[50];
  char jsonAtributos[200];
  if(xSemaphoreTake(connectionMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       const char* alarme = "alarme";   
       int32_t ALARME=le_valor_nvs(alarme);
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
    int MODO_OTA=0;
    if(MODO_OTA==1){
    simple_ota_example();
    }
    const char* alarme = "alarme";   
    int32_t ALARME=le_valor_nvs(alarme);
    printf("Valor do alarme %ld",ALARME);
    if(ALARME==1){
      security();
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
    xTaskCreate(&handle_server_communication, "Comunicação com Broker", 4096, NULL, 1, NULL);
}



