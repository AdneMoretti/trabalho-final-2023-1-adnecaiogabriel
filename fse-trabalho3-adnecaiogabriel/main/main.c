#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

// Configuração dos pinos
#define SENSOR_PIN 4
#define RED_PIN 23
#define GREEN_PIN 22
#define BLUE_PIN 21
#define BUTTON_PIN 0

// Variáveis de controle
volatile bool is_alert_active = false;

// Função para piscar o LED na cor vermelha
void blink_red(void *pvParameters) {
    while (1) {
        if (is_alert_active) {
            printf("BLINK\n");
            gpio_set_level(RED_PIN, 1);
            gpio_set_level(GREEN_PIN, 0);
            gpio_set_level(BLUE_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(RED_PIN, 1);
            gpio_set_level(GREEN_PIN, 1);
            gpio_set_level(BLUE_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
        } else {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

// Função para tratar o evento de clique no botão
void IRAM_ATTR button_isr_handler(void* arg) {
    is_alert_active = false;
}

void app_main() {
    // Configuração dos pinos
    esp_rom_gpio_pad_select_gpio(SENSOR_PIN);
    gpio_set_direction(SENSOR_PIN, GPIO_MODE_INPUT);

    esp_rom_gpio_pad_select_gpio(RED_PIN);
    gpio_set_direction(RED_PIN, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(GREEN_PIN);
    gpio_set_direction(GREEN_PIN, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(BLUE_PIN);
    gpio_set_direction(BLUE_PIN, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_PIN, button_isr_handler, NULL);

    // Cria a tarefa para piscar o LED
    xTaskCreatePinnedToCore(blink_red, "blink_red_task", 2048, NULL, 1, NULL, 0);

    while (1) {
        // Verifica se o nível do som está acima do limiar desejado (ajuste o valor conforme necessário)
        if (gpio_get_level(SENSOR_PIN) > 0) {
            is_alert_active = true;
        }
        // printf("LOOP\n");
        // is_alert_active = true;

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
