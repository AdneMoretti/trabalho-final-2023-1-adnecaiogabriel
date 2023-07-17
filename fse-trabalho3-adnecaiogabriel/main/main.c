// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <driver/gpio.h>

// // Configuração dos pinos
// #define SENSOR_PIN 19
// #define RED_PIN 23
// #define GREEN_PIN 22
// #define BLUE_PIN 21
// #define BUTTON_PIN 0

// // Variáveis de controle
// volatile bool is_alert_active = false;
// volatile bool is_sound_active = false;

// // Função para piscar o LED na cor vermelha
// void blink_red(void *pvParameters) {
//     while (1) {
//         if (is_alert_active) {
//             printf("BLINK\n");
//             gpio_set_level(RED_PIN, 0);
//             gpio_set_level(GREEN_PIN, 1);
//             gpio_set_level(BLUE_PIN, 1);
//             vTaskDelay(pdMS_TO_TICKS(500));
//             gpio_set_level(RED_PIN, 1);
//             gpio_set_level(GREEN_PIN, 1);
//             gpio_set_level(BLUE_PIN, 1);
//             vTaskDelay(pdMS_TO_TICKS(500));
//         } else {
//             gpio_set_level(RED_PIN, 1);
//             gpio_set_level(GREEN_PIN, 1);
//             gpio_set_level(BLUE_PIN, 1);
//             vTaskDelay(pdMS_TO_TICKS(10));
//         }
//     }
// }

// void detect_sound(void *pvParameters) {
//     while (1) {
//         if (is_sound_active) {
//             printf("SOUND\n");
//             vTaskDelay(pdMS_TO_TICKS(500));
//         } else {
//             vTaskDelay(pdMS_TO_TICKS(10));
//         }
//     }
// }

// // Função para tratar o evento de clique no botão
// void IRAM_ATTR button_isr_handler(void* arg) {
//     is_alert_active = false;
// }

// void app_main() {
//     // Configuração dos pinos
//     esp_rom_gpio_pad_select_gpio(SENSOR_PIN);
//     gpio_set_direction(SENSOR_PIN, GPIO_MODE_INPUT);

//     esp_rom_gpio_pad_select_gpio(RED_PIN);
//     gpio_set_direction(RED_PIN, GPIO_MODE_OUTPUT);

//     esp_rom_gpio_pad_select_gpio(GREEN_PIN);
//     gpio_set_direction(GREEN_PIN, GPIO_MODE_OUTPUT);

//     esp_rom_gpio_pad_select_gpio(BLUE_PIN);
//     gpio_set_direction(BLUE_PIN, GPIO_MODE_OUTPUT);

//     esp_rom_gpio_pad_select_gpio(BUTTON_PIN);
//     gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
//     gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

//     // Cria a tarefa para piscar o LED
//     xTaskCreatePinnedToCore(blink_red, "blink_red_task", 2048, NULL, 1, NULL, 0);
//     xTaskCreatePinnedToCore(detect_sound, "detect_sound_task", 2048, NULL, 1, NULL, 0);

//     while (1) {
//         // Verifica se o nível do som está acima do limiar desejado (ajuste o valor conforme necessário)
//         int sensorValue = gpio_get_level(SENSOR_PIN);
//         printf("Valor do sensor: %d\n", sensorValue);
        
//         printf("LOOP\n");
//         // is_alert_active = true;

//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
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
        //   readShockSensorBatery();
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
      } else if(ESP_CONFIG_NUMBER == 1) {

      } else if(ESP_CONFIG_NUMBER == 2) {
        configure_SOUND();
        xTaskCreate(&check_sound, "Leitura Sensor de Som", 4096, NULL, 1, NULL);
      } else {
        printf("ESP not identified");
      }
    }
}
