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
#include "mqtt.h"
#include "global.h"
#include "driver/gpio.h"
#include "json_parser.h"
#include "nvs.h"
#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER

#define TAG "MOSQUITTO"

extern SemaphoreHandle_t connectionMQTTSemaphore;
esp_mqtt_client_handle_t client;

// void stop_alarm()
// {
//     grava_valor_nvs(0,"alarme");
// }

// void mosquitto_event_data_parser(char *data)
// {
//     cJSON *json = cJSON_Parse(data);
//     if (json == NULL)
//         return;

//     int alerta = cJSON_GetObjectItem(json, "Alerta")->valueint;
//     char *tag = cJSON_GetObjectItem(json, "TAG")->valuestring;

//     if(alerta == 1){
//         xTaskCreate(&security, "Ativar segurança", 4096, NULL, 1, NULL);
//     } else {
//         stop_alarm();
//     }
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
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            xSemaphoreGive(connectionMQTTSemaphore);
            esp_mqtt_client_subscribe(client, "FSEACG/alarme", 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // mosquitto_envia_mensagem("FSEACG/alarme", "1");
            // message = create_message_json(data.temperature, data.humidity, data.magnetic_signal);
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
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            
            if (ESP_CONFIG_NUMBER == 1 || ESP_CONFIG_NUMBER == 2) {
                // mosquitto_event_data_parser(event->data);
            }
            
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
    int message_id = esp_mqtt_client_publish(client, topico, mensagem, 0, 0, 0);
    ESP_LOGI(TAG, "Mensagem enviada, ID: %d", message_id);
}
