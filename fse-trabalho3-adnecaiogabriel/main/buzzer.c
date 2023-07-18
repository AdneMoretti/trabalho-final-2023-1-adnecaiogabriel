#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/ledc.h>

// Definição das notas musicais
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
// ... adicione as outras notas aqui

// Frequência do PWM do buzzer
#define BUZZER_PWM_FREQ 2000

// Configuração do buzzer
#define BUZZER_PIN  2
#define BUZZER_CHANNEL LEDC_CHANNEL_0

// Estrutura para armazenar as informações da nota
typedef struct {
    int note;
    int duration;
} Note;

// Função para reproduzir uma nota no buzzer
void playNote(Note note) {
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, BUZZER_CHANNEL, note.note);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, BUZZER_CHANNEL);
    vTaskDelay(pdMS_TO_TICKS(note.duration));
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, BUZZER_CHANNEL, 0);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, BUZZER_CHANNEL);
    vTaskDelay(pdMS_TO_TICKS(50)); // Pequena pausa entre as notas
}

// Tarefa para tocar a música
void playMusic(void *pvParameters) {

	    // Configuração do canal LEDC para o buzzer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = BUZZER_PWM_FREQ,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = BUZZER_CHANNEL,
        .duty = 0,
        .gpio_num = BUZZER_PIN,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);

    // Aqui você pode definir a sua música
    Note music[] = {
        {NOTE_C1, 500},
        {NOTE_D1, 500},
        {NOTE_E1, 500},
        // ... adicione as outras notas e durações da música aqui
    };
    
    size_t numNotes = sizeof(music) / sizeof(Note);

    for (size_t i = 0; i < numNotes; i++) {
        playNote(music[i]);
    }

}



// void app_main() {
//     // Configuração do canal LEDC para o buzzer
//     ledc_timer_config_t ledc_timer = {
//         .duty_resolution = LEDC_TIMER_13_BIT,
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

//     // Criação da tarefa para tocar a música
//     xTaskCreatePinnedToCore(playMusic, "playMusic", configMINIMAL_STACK_SIZE, NULL, 5, NULL, APP_CPU_NUM);
// }


void play_buzzer(){
	gpio_set_level(BUZZER_PIN, 1);

}