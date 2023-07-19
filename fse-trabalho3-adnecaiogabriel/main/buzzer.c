// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/ledc.h"

// #define BUZZER_PIN  2
// #define BUZZER_CHANNEL 0
// #define BUZZER_PWM_FREQ 2000
// #define BUZZER_RESOLUTION LEDC_TIMER_8_BIT

// void setup() {
//     ledc_timer_config_t ledc_timer = {
//         .duty_resolution = BUZZER_RESOLUTION,
//         .freq_hz = BUZZER_PWM_FREQ,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .timer_num = LEDC_TIMER_0
//     };
//     ledc_timer_config(&ledc_timer);

//     ledc_channel_config_t ledc_channel = {
//         .channel = BUZZER_CHANNEL,
//         .duty = 0,
//         .gpio_num = BUZZER_PIN,
//         .speed_mode = LEDC_HIGH_SPEED_MODE,
//         .timer_sel = LEDC_TIMER_0
//     };
//     ledc_channel_config(&ledc_channel);

//     // Start serial communication
//     // Set the frequency to the initial value (2000 Hz)
//     ledcWrite(BUZZER_CHANNEL, BUZZER_PWM_FREQ);
// }

// void playMusic() {
//     // Vary the duty cycle
//     for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += 10) {
//         printf("Duty Cycle: %d\n", dutyCycle);
//         ledcWrite(BUZZER_CHANNEL, dutyCycle);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }

//     // Fix the duty cycle to 125 (approximately 50%)
//     ledcWrite(BUZZER_CHANNEL, 125);

//     // Vary the frequency
//     for (int freq = 255; freq < 10000; freq += 250) {
//         printf("Frequency: %d\n", freq);
//         ledcWriteTone(BUZZER_CHANNEL, freq);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }


// // void app_main() {
// //     // Inicialize o FreeRTOS e crie a tarefa para tocar a música
// //     xTaskCreate(playMusic, "playMusic", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);

// //     // Inicie o escalonador do FreeRTOS
// //     vTaskStartScheduler();
// // }


// // void app_main() {
// //     // Criação da tarefa para tocar a música
// //     xTaskCreatePinnedToCore(playMusic, "playMusic", configMINIMAL_STACK_SIZE, NULL, 5, NULL, APP_CPU_NUM);
// // }


// // void app_main() {
// //     // Configuração do canal LEDC para o buzzer
// //     ledc_timer_config_t ledc_timer = {
// //         .duty_resolution = LEDC_TIMER_13_BIT,
// //         .freq_hz = BUZZER_PWM_FREQ,
// //         .speed_mode = LEDC_HIGH_SPEED_MODE,
// //         .timer_num = LEDC_TIMER_0
// //     };
// //     ledc_timer_config(&ledc_timer);

// //     ledc_channel_config_t ledc_channel = {
// //         .channel = BUZZER_CHANNEL,
// //         .duty = 0,
// //         .gpio_num = BUZZER_PIN,
// //         .speed_mode = LEDC_HIGH_SPEED_MODE,
// //         .timer_sel = LEDC_TIMER_0
// //     };
// //     ledc_channel_config(&ledc_channel);

// //     // Criação da tarefa para tocar a música
// //     xTaskCreatePinnedToCore(playMusic, "playMusic", configMINIMAL_STACK_SIZE, NULL, 5, NULL, APP_CPU_NUM);
// // }


// void play_buzzer(){
// 	gpio_set_level(BUZZER_PIN, 1);

// }