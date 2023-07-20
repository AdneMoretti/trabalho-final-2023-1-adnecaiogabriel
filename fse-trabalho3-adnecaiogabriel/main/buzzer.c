#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

static ledc_timer_config_t ledcTimer;
static ledc_channel_config_t ledc_channel;


// void configure_buzzer_pwm(gpio_num_t pin) {
//     // ledc_timer_config_t ledcTimer;

//     ledcTimer.duty_resolution = LEDC_TIMER_8_BIT;
//     ledcTimer.freq_hz = 5000;
//     ledcTimer.speed_mode = LEDC_HIGH_SPEED_MODE;
//     ledcTimer.timer_num = LEDC_TIMER_0;
//     ledc_timer_config(&ledcTimer);

//     ledc_channel.channel = LEDC_CHANNEL_0;
//     ledc_channel.duty = 0;
//     ledc_channel.gpio_num = pin;
//     ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
//     ledc_channel.hpoint = 0;
//     ledc_channel.timer_sel = LEDC_TIMER_0;
//     ledc_channel_config(&ledc_channel);
// }

// void buzzer_control(uint32_t frequency) {
//     // Ensure the frequency is within the range supported by the LEDC module
//     if (frequency < 10) {
//         frequency = 10;
//     } else if (frequency > 10000) {
//         frequency = 10000;
//     }

//     // Calculate the duty cycle required to achieve the desired frequency
//     uint32_t duty = (1 << ledcTimer.duty_resolution) / frequency / 2;

//     ledc_set_duty(ledcTimer.speed_mode, ledc_channel.channel, duty);
//     ledc_update_duty(ledcTimer.speed_mode, ledc_channel.channel);
// }

// // Example usage
// void buzzer() {
//     gpio_set_level(2, 1);
//     configure_buzzer_pwm(GPIO_NUM_2);

//     // Change the buzzer frequency to 2000 Hz
//     buzzer_control(2000);
//     vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for 2 seconds

//     // Change the buzzer frequency to 5000 Hz
//     buzzer_control(5000);
//     vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for 2 seconds

//     // Change the buzzer frequency to 1000 Hz
//     buzzer_control(1000);
//     vTaskDelay(pdMS_TO_TICKS(2000)); // Wait for 2 seconds
// }

void buzzer(){
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
}

void buzzer1(){
    gpio_set_level(2, 1);
    vTaskDelay(500 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(1000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
}

void buzzer2(){
    gpio_set_level(2, 1);
    vTaskDelay(2000 /  portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
}