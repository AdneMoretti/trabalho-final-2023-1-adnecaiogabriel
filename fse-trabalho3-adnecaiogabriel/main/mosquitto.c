#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include <ctype.h>
#include "mosquitto.h"
#include "cJSON.h"

#define FLAG_ALARME 0 
#define TAG "MOSQUITTO"

extern SemaphoreHandle_t connectionMQTTSemaphore;
esp_mqtt_client_handle_t client;

#include "driver/gpio.h"
#define BUZZER_GPIO 2
#define SENSOR 15

cJSON* message;

cJSON* create_message_json(float temperature, float humidity, int magnetic_signal) {
    cJSON* functionJSON = cJSON_CreateObject();
    cJSON_AddNumberToObject(functionJSON, "temperature", temperature);
    cJSON_AddNumberToObject(functionJSON, "humidity", humidity);
    cJSON_AddNumberToObject(functionJSON, "parameters", magnetic_signal);
    return functionJSON;
}

// void security(int ALARME)
// {
//   // Configuração dos pinos dos LEDs 
//   esp_rom_gpio_pad_select_gpio(BUZZER_GPIO);   
//   esp_rom_gpio_pad_select_gpio(SENSOR);
//   // Configura os pinos dos LEDs como Output
//   gpio_set_direction(SENSOR, GPIO_MODE_INPUT);
//   gpio_set_direction(BUZZER_GPIO, GPIO_MODE_OUTPUT);  

//   // Configuração do pino do Botão
//   // Configura o pino do Botão como Entrada
//   // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)  
//   gpio_set_level(BUZZER_GPIO,0);
//     ALARME=ALARME;
//   // Testa o Botão utilizando polling
//   while (1){
//     if(gpio_get_level(SENSOR) == 1){ //SE A LEITURA DO SENSOR FOR IGUAL A HIGH, FAZ
//       printf("Desligou alarme");
//       gpio_set_level(BUZZER_GPIO, 0); //desliga o alarme
//       vTaskDelay(100 / portTICK_PERIOD_MS);
//       break;
//     }else if(gpio_get_level(SENSOR) == 0){ //SE A LEITURA DO SENSOR FOR IGUAL A LOW, FAZ
//       printf("Ligado o Alarme");
//       gpio_set_level(BUZZER_GPIO, 1); //ACENDE O alarme
//       vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
//   }
// }
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
     ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, (int) event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            xSemaphoreGive(connectionMQTTSemaphore);
            msg_id = esp_mqtt_client_subscribe(client, "FSEACG/alarme", 0);

            // char jsonAtributos[200];
            // sprintf(jsonAtributos, "{\"temperatura\": 123, \n\"umidade\": 20}");
            // char response[200];
            // sprintf(response, "v1/devices/me/attributes/request/%d", response_id);
            // mqtt_envia_mensagem(response, '{"clientKeys":"temperatura"}');
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // message = create_message_json(data.temperature, data.humidity, data.magnetic_signal);
            mosquitto_envia_mensagem("FSEACG/alarme", "1");
            // mosquitto_envia_mensagem("FSEACG/alarme", "0");


            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            
            // mqtt_event_data_parser(event->data, event->topic);
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            // char* comp="1";
            // printf("%s",event->data);
            // if(event->data[0]=='1'){
            //     security(1);
            // }else{
            //     security(0);
            // }
            // transformString(event->topic);
            // printf("%s",event->topic);
            // char jsonAtributos[200];
            // sprintf(jsonAtributos, "{\"temperatura\": 123, \n\"umidade\": 20}");
            // mqtt_envia_mensagem(event->topic,jsonAtributos);
            // mqtt_envia_mensagem("v1/devices/me/attributes/request/1","{\"clientKeys\":\"temperatura\"}");

            // int response_id = esp_mqtt_client_subscribe(client,"v1/devices/me/attributes/response/+",0);
            // char response[200];
            // sprintf(response, "v1/devices/me/attributes/request/%d", response_id);
            // mqtt_envia_mensagem(response, '{"clientKeys":"temperatura"}');
            // int num =find_integers(event->topic);
            // char local[50];
            // char str[50];
            // sprintf(str,"%d",num);
            // strcpy(local, "v1/devices/me/rpc/response/");
            // strcat(local,str);
            // printf("%s",local);
            // mqtt_envia_mensagem(local,"{ledValor: 1}");
            // mqtt_envia_mensagem(strcat("v1/devices/me/rpc/response/",num),"34");
            // char* openingBrace = strchr(receivedString, '{');
            // char requestId[100];
            // strcpy(requestId, event->topic + strlen("v1/devices/me/rpc/request/"));
            // esp_mqtt_client_publish(client, "v1/devices/me/rpc/response/", requestId, strlen(requestId), 0, false);
            // mqtt_envia_mensagem()
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
}
}

void mosquitto_start()
{
    esp_mqtt_client_config_t mqtt_config = {
        .broker.address.uri = "mqtt://test.mosquitto.org"
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mosquitto_envia_mensagem(char * topico, char * mensagem)
{
    int message_id = esp_mqtt_client_publish(client, topico, mensagem, 0, 1, 0);
    ESP_LOGI(TAG, "Mesnagem enviada, ID: %d", message_id);
}
