#include <stdio.h>
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