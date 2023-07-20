#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "buzzer.h"
#include "gpio_setup.h"
#include "json_parser.h"
#include <driver/ledc.h>

#define TAG "HALL"
#define HALL_PIN 15

void verifica_magnetic()
{
    // printf("entrei aqui");
    int magnetic;
    configure_BUZZER();
    // setup();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    while(1){
        magnetic = gpio_get_level(HALL_PIN);
        send_magnetic_signal(magnetic);

        ESP_LOGI(TAG, "campo magnetico %d", magnetic);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// int readHallBatery(){
//     int magnetic_signal;
//     int last_magnetic_signal = 1;
//     magnetic_signal = gpio_get_level(HALL_PIN);
//     if (magnetic_signal == 0) {
//         printf("Status antigo sensor de magnetismo: %d\n", last_magnetic_signal);
//         printf("Status atual sensor de magnetismo: %d", magnetic_signal);
//         send_magnetic_signal(magnetic_signal);
//         return 1;
//     }
//     // last_magnetic_signal = magnetic_signal;
//     return 0;
// }