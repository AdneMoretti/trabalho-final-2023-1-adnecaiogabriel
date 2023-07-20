#include "cJSON.h"
#include "cJSON.c"
#include "mqtt.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "mosquitto.h"

#define TAG_MAGNETIC "MAGNETIC_ALARM"
#define TAG "MQTT"

void set_attributes_states(char key, int value, int topic_id)
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

void send_temperature_telemetry(int temperature, int humidity)
{
    double temperature_d, humidity_d;
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }
    temperature_d = (double) temperature;
    humidity_d = (double) humidity;
    cJSON_AddNumberToObject(root, "temperature", temperature_d);
    cJSON_AddNumberToObject(root, "humidity", humidity_d);    
    printf("%s", cJSON_Print(root));
    mqtt_envia_mensagem("v1/devices/me/telemetry", cJSON_Print(root));
    mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(root));
}

void send_magnetic_signal(int magnetic_signal)
{

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }
    if(!magnetic_signal){
        cJSON *alarm = cJSON_CreateObject();
        cJSON_AddNumberToObject(alarm, "Alerta", 1);
        cJSON_AddStringToObject(alarm, "TAG", TAG_MAGNETIC);
        mosquitto_envia_mensagem("FSEACG/alarme", cJSON_Print(alarm));
    }

    cJSON_AddNumberToObject(root, "magnetic_signal", magnetic_signal);  
    mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(root));
}