#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "json_parser.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "cJSON.h"
#include "global.h"
#include "mqtt.h"

#define TAG "MQTT"
#define ESP_CONFIG_NUMBER CONFIG_ESP_CONFIG_NUMBER

extern SemaphoreHandle_t connectionMQTTSemaphore;
esp_mqtt_client_handle_t client_dash;
esp_mqtt_client_handle_t client_esp;

char *method;
int value;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}
void transformString(char* input) {
    // Find the position of the opening brace '{'
    char* brace = strchr(input, '{');
    if (brace != NULL) {
        *brace = '\0'; // Truncate the string at the position of '{'
    }
    
    // Find the position of the last '/'
    char* lastSlash = strrchr(input, '/');
    if (lastSlash != NULL) {
        char requestId[100];
        strcpy(requestId, lastSlash + 1);
        sprintf(input, "v1/devices/me/rpc/response/%s", requestId);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, (int) event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client_dash = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            xSemaphoreGive(connectionMQTTSemaphore);
            msg_id = esp_mqtt_client_subscribe(client_dash, "v1/devices/me/rpc/request/+", 0);
            ESP_LOGI(TAG, "sent suscribe succesfull, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client_dash,"v1/devices/me/attributes/response/+",0);
            ESP_LOGI(TAG, "sent suscribe succesfull, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
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
            if(ESP_CONFIG_NUMBER == 0){
                // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
                printf("DATA=%.*s\r\n", event->data_len, event->data);
                ESP_LOGI(TAG, "MQTT_EVENT_DATA");

                cJSON *json = cJSON_Parse(event->data);
                if (json == NULL)
                    return;

                method = cJSON_GetObjectItem(json, "method")->valuestring;
                if(strcmp(method, "setAlarm")==0){
                    value = cJSON_GetObjectItem(json, "params")->valueint;
                    send_dashboard_signal(value);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);

                }

                // transformString(event->topic);
                // printf("%s",event->topic);
                // char jsonAtributos[200];
                // sprintf(jsonAtributos, "{\"magnetic_signal\": 0}");
                // printf("\n%s",jsonAtributos);
                // mqtt_envia_mensagem(event->topic,jsonAtributos);
                
            }

            // void mosquitto_event_data_parser(char* data)
            // char requestId[100];
            // strcpy(requestId, event->topic + strlen("v1/devices/me/rpc/request/"));
            // esp_mqtt_client_publish(client, "v1/devices/me/rpc/response/", requestId, strlen(requestId), 0, false);
            // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            // printf("DATA=%.*s\r\n", event->data_len, event->data);

            if (ESP_CONFIG_NUMBER ==1){
<<<<<<< HEAD
                transformString(event->topic);
                printf("%s",event->topic);
                char jsonAtributos[200];
                sprintf(jsonAtributos, "{\"alarme\": 1}");
                printf("\n%s",jsonAtributos);
                mqtt_envia_mensagem(event->topic,jsonAtributos);
=======
            transformString(event->topic);
            printf("%s",event->topic);
            char jsonAtributos[200];
            sprintf(jsonAtributos, "{\"alarme\": true}");
            printf("\n%s",jsonAtributos);
            mqtt_envia_mensagem("v1/devices/me/attributes", jsonAtributos);
>>>>>>> 7c6ee367c3c293eeedbd11d959949274aa892587
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

void mqtt_start()
{   
    char *token_dash;
    if (ESP_CONFIG_NUMBER == 0){
      token_dash = "XgQhdKfAdnzug96MmPvQ";
    } else if (ESP_CONFIG_NUMBER == 1) {
      token_dash = "1MklVxb6sYtA5Fw145yX";
    } else if (ESP_CONFIG_NUMBER == 2) {
      token_dash = "ZodDWt0knISJC2CLkRir";
    };
    esp_mqtt_client_config_t mqtt_config = {
          .broker.address.uri = "mqtt://164.41.98.25",
          .credentials.username = token_dash
    };
    client_dash = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client_dash, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client_dash);
}

void mqtt_envia_mensagem(char * topico, char * mensagem)
{
    int message_id = esp_mqtt_client_publish(client_dash, topico, mensagem, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem enviada, ID: %d %s", message_id, topico);
}

void mqtt_stop(){
    esp_err_t err = esp_mqtt_client_stop(client_dash);
    ESP_ERROR_CHECK(err);
}