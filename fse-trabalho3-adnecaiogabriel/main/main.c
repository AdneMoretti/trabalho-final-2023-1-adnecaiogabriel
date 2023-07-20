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
// #include "sound_sensor.h"
#include "gpio_setup.h"
#include <math.h>
#include "global.h"
#include "light_sleep.h"
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
      // mqtt_start();
    }
  }
}

// void handle_server_communication(void * params)
// {
//   char mensagem[50];
//   char jsonAtributos[200];
//   if(xSemaphoreTake(connectionMQTTSemaphore, portMAX_DELAY))
//   {
//     while(true)
//     {
//        float temp = 20.0 + (float)rand()/(float)(RAND_MAX/10.0);
       
      //  sprintf(mensagem, "{\"temperature\": %f}", temp);
      //  mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);

      //  sprintf(jsonAtributos, "{\"quantidade de pinos\": 5, \n\"umidade\": 20}");
      //  mqtt_envia_mensagem("v1/devices/me/attributes", jsonAtributos);

//        vTaskDelay(3000 / portTICK_PERIOD_MS);
//     }
//   }
// }

float limit_decimal(float x, int decimal_places){
  float power = pow(10, decimal_places);
  return roundf(x*power)/power;
}

void app_main(void)
{    
  // xTaskCreate(&security,"conex",4096,1,1,NULL);
    int MODO_OTA=0;
    if(MODO_OTA==1){
    simple_ota_example();
    }
    const char* alarme = "alarme";    
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
    int32_t ALARME=le_valor_nvs(alarme);
    ALARME=1;
    grava_valor_nvs(ALARME,alarme);
    int32_t caio=le_valor_nvs(alarme);
    printf("%ld",caio);










    // grava_valor_nvs(valor_lido,variable);
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //   ESP_ERROR_CHECK(nvs_flash_erase());
    //   ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);
    // connectionWifiSemaphore = xSemaphoreCreateBinary();
    // connectionMQTTSemaphore = xSemaphoreCreateBinary();
    // reconnectionWifiSemaphore = xSemaphoreCreateBinary();
    // wifi_start();

    // xTaskCreate(&wifi_connected,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    
    // xTaskCreate(&handle_server_communication, "Comunicação com Broker", 4096, NULL, 1, NULL);

    if(ESP_MODE == BATTERY_MODE) {
        if(ESP_CONFIG_NUMBER == 0) {
        //wake_up_with_gpio(16);
        wake_up_with_hall();
        configure_HALL();
        printf("aqui");
            while(1){
                ESP_LOGI("Modo Funcionamento", "Bateria");
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                // readHallBatery();
                light_sleep_task();
                // xTaskCreate(&wifi_connected,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
            }
    } else {
        printf("ESP not identified");
    }
}
    else if(ESP_MODE == ENERGY_MODE) {
      ESP_LOGI("Modo Funcionamento", "ENERGIA");
      
      if(ESP_CONFIG_NUMBER == 0) {
        xTaskCreate(&verifica_magnetic, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
        xTaskCreate(&check_temperature, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
        // xTaskCreate(&check_temperature, "Verificando existencia de campo magnetico", 4096, NULL, 1, NULL);
      } else if(ESP_CONFIG_NUMBER == 1) {
        
      // } else if(ESP_CONFIG_NUMBER == 2) {
      //   configure_SOUND();
      //   xTaskCreate(&check_sound, "Leitura Sensor de Som", 4096, NULL, 1, NULL);
      } else {
        printf("ESP not identified");
      }
    }
}



// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <time.h>
// #include <sys/time.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_sleep.h"
// #include "esp_log.h"
// #include "esp32/rom/uart.h"
// #include "driver/gpio.h"

// // Driver da GPIO com funções estendidas para o modo sleep
// #include "driver/rtc_io.h"

// #define BOTAO 0

// void app_main()
// {
//     // Configuração da GPIO para o botão de entrada
//     esp_rom_gpio_pad_select_gpio(BOTAO);
//     gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
//     // Habilita o botão para acordar a placa
//     gpio_wakeup_enable(BOTAO, GPIO_INTR_LOW_LEVEL);
//     esp_sleep_enable_gpio_wakeup();

//     // Configurando o Sleep Timer (em microsegundos)
//     esp_sleep_enable_timer_wakeup(5 * 1000000);

//   while(true)
//   {

//     if (rtc_gpio_get_level(BOTAO) == 0)
//     {
//         printf("Aguardando soltar o botão ... \n");
//         do
//         {
//             vTaskDelay(pdMS_TO_TICKS(10));
//         } while (rtc_gpio_get_level(BOTAO) == 0);
//     }

//     printf("Entrando em modo Light Sleep\n");
    
//     // Configura o modo sleep somente após completar a escrita na UART para finalizar o printf
//     uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

//     // int64_t tempo_antes_de_dormir = esp_timer_get_time();

//     // Entra em modo Light Sleep
//     esp_light_sleep_start();

//     // int64_t tempo_apos_acordar = esp_timer_get_time();

//     esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();

//     // printf("Dormiu por %lld ms\n", (tempo_apos_acordar - tempo_antes_de_dormir) / 1000);
//     printf("O [%s] me acordou !\n", causa == ESP_SLEEP_WAKEUP_TIMER ? "TIMER" : "BOTÃO");

//   }

// }
