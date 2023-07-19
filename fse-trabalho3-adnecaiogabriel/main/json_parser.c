#include "cJSON.h"
#include "cJSON.c"
#include "mqtt.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "mosquitto.h"

#define TAG "MQTT"
#define SOUND_ALARM "SOUND_ALARM"

void set_attributes_states(char *key, int value, int topic_id)
{
}

void mqtt_event_data_parser(char *data, char *topic)
{
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
        return;

    int rst;
    int topic_id;
    rst = sscanf(topic, "v1/devices/me/rpc/request/%d",
      &topic_id);
    char *key = cJSON_GetObjectItem(json, "method")->valuestring;
    int value = cJSON_GetObjectItem(json, "params")->valueint;
    if(strstr(key, "set") != NULL){
        set_attributes_states(key, value, topic_id);
    }

    printf("State");
}

void mosquitto_event_data_parser(char *data)
{
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
        return;

    int alerta = cJSON_GetObjectItem(json, "Alerta")->valueint;
    char *tag = cJSON_GetObjectItem(json, "TAG")->valuestring;

    if(alerta == 1){
        security(tag);
    }
}

void send_sound(int *sound)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }
    
    double sound_toDouble = *(int *)sound;

    if (sound_toDouble == 1) {
        cJSON *alarm = cJSON_CreateObject();
        if (alarm == NULL)
        {
            ESP_LOGE(TAG, "Não foi possível criar o JSON");
            return;
        }

        cJSON_AddNumberToObject(alarm, "Alerta", 1);
        cJSON_AddStringToObject(alarm, "TAG", SOUND_ALARM);
        mosquitto_envia_mensagem("FSEACG/alarme", cJSON_Print(alarm));
    }

    cJSON_AddNumberToObject(root, "sound", sound_toDouble);
    mqtt_envia_mensagem("v1/devices/me/telemetry", cJSON_Print(root));
}