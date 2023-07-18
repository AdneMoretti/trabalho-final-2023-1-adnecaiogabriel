#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"

static ledc_channel_config_t ledc_channel;

void configure_led_pwm(gpio_num_t pin) {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = pin;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&ledc_channel);
}

void led_control(int brightness) {
    if (brightness < 0){
        brightness = 0;
    }
    else if (brightness > 1020){
        brightness = 1020;
    }

    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, brightness);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}
=======
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define LED_1 2

void app_main()
{
  
  // Configuração do Timer
  ledc_timer_config_t timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 1000,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_config);

  // Configuração do Canal
  ledc_channel_config_t channel_config = {
    .gpio_num = LED_1,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  };
  ledc_channel_config(&channel_config);

  // Utilizando o PWM
  // while(true)
  // {
  //   for(int i = 0; i < 255; i++)
  //   {
  //     ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
  //     ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
  //     vTaskDelay(10 / portTICK_PERIOD_MS);
  //   }

  //   for(int i = 255; i > 0; i--)
  //   {
  //     ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
  //     ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
  //     vTaskDelay(10 / portTICK_PERIOD_MS);
  //   }
  // }

  ledc_fade_func_install(0);
  while(true)
  {
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 1000, LEDC_FADE_WAIT_DONE);
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 255, 1000, LEDC_FADE_WAIT_DONE);
  }
  


}
>>>>>>> 8d538a7ede48e9cc3f5f8277fb08253f0a0a5e6d
