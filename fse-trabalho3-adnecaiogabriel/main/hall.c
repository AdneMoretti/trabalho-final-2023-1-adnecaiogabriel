# include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "adc_module.h"

#define HALL_PIN 22

int get_magnetic(){
    int magnetic;
    magnetic = digitalRead(HALL_PIN);
    return magnetic;
}

void verifica_magnetic(){
    int magnetic;
    magnetic = get_magnetic();
    if(magnetic){
        //ATIVA O BUZZER? 
        printf("%d", magnetic);
    }
}