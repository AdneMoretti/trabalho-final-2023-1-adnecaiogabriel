#include "cJSON.h"
#include "cJSON.c"
#include "mqtt.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"

#define TAG "MQTT"

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

void send_sound_alert(int *sound)
{

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }

    double sound_toDouble = *(int *)sound;
    
    cJSON_AddItemToObject(root, "Alerta de Som", cJSON_CreateNumber(sound_toDouble));
    mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(root));
}

void send_sound_telemetry(int *sound)
{

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }

    double sound_toDouble = *(int *)sound;
    
    cJSON_AddItemToObject(root, "Sensor de Som", cJSON_CreateNumber(sound_toDouble));
    mqtt_envia_mensagem("v1/devices/me/telemetry", cJSON_Print(root));
}