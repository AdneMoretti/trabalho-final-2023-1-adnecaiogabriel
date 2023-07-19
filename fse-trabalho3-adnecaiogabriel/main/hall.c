#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "buzzer.h"
#include "gpio_setup.h"
#include "json_parser.h"

#define TAG "HALL"
#define HALL_PIN 15

void verifica_magnetic()
{
    // printf("entrei aqui");
    int magnetic;
    configure_BUZZER();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    while(1){
        magnetic = gpio_get_level(HALL_PIN);
        if(!magnetic){
            send_magnetic_signal(magnetic);
        }
        ESP_LOGI(TAG, "campo magnetico %d", magnetic);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}