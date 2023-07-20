#include "cJSON.h"
#include "cJSON.c"
#include "mqtt.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "mosquitto.h"
#include "nvs.h"

#define TAG "MQTT"
#define SOUND_ALARM "SOUND_ALARM"
#define TAG_MAGNETIC "MAGNETIC_ALARM"
#define TAG_DASHBOARD "DASHBOARD_ALARM"

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

    printf("State");
}

void stop_alarm()
{
    grava_valor_nvs(0,"alarme");
}

void mosquitto_event_data_parser(char *data)
{
    cJSON *json = cJSON_Parse(data);
    if (json == NULL)
        return;

    int alerta = cJSON_GetObjectItem(json, "Alerta")->valueint;
    char *tag = cJSON_GetObjectItem(json, "TAG")->valuestring;

    if(alerta == 1){
        xTaskCreate(&security, "Ativar segurança", 4096, (void*)tag, 1, NULL);
    } else {
        stop_alarm();
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

void send_stop_alarm(char *ALARM_TAG)
{
    cJSON *alarm = cJSON_CreateObject();
    if (alarm == NULL)
    {
        ESP_LOGE(TAG, "Não foi possível criar o JSON");
        return;
    }

    cJSON_AddNumberToObject(alarm, "Alerta", 0);
    cJSON_AddStringToObject(alarm, "TAG", ALARM_TAG);
    mosquitto_envia_mensagem("FSEACG/alarme", cJSON_Print(alarm));
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

void send_dashboard_signal(int signal)
{
    cJSON *alarm = cJSON_CreateObject();
    cJSON_AddNumberToObject(alarm, "Alerta", signal);
    cJSON_AddStringToObject(alarm, "TAG", TAG_DASHBOARD);
    mosquitto_envia_mensagem("FSEACG/alarme", cJSON_Print(alarm));

}