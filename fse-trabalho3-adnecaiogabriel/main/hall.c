#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "buzzer.h"
#include "gpio_setup.h"
#define TAG "HALL"
#define HALL_PIN 15

int get_magnetic(){
    int magnetic;
    magnetic = gpio_get_level(HALL_PIN);
    return magnetic;
}

void verifica_magnetic(){
    // printf("entrei aqui");
    int magnetic;
    configure_BUZZER();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    while(1){
        magnetic = get_magnetic();
        if(!magnetic){
            play_buzzer();
        }
        ESP_LOGI(TAG, "campo magnetico %d", magnetic);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    // if(magnetic){
    //     //ATIVA O BUZZER? 
    //     // printf("%d", magnetic);
    // }
    }
}