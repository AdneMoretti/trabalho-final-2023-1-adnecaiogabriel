// #include "cJSON.h"
// #include "cJSON.c"
// #include "mqtt.h"
// #include "esp_log.h"
// #include "esp_event.h"
// #include "mqtt_client.h"
// #include "heartbeat_module.h"

// #define TAG "MQTT"

// void send_humidity_telemetry(void *temperature, void *humidity)
// {

// }

// void mqtt_event_data_parser(char *data, char *topic)
// {

// }

// void send_board_magnetic_attribute(int *magnetic_status)
// {

// }

// void send_board_led_attribute(int *led_status)
// {
//     cJSON *root = cJSON_CreateObject();

//     float ls = *(int *)led_status;
    
//     cJSON_AddItemToObject(root, "led da placa", cJSON_CreateNumber(ls));
//     mqtt_envia_mensagem("v1/devices/me/attributes", cJSON_Print(root));
// }