<<<<<<< HEAD
#ifndef GPIO_SETUP_H
#define GPIO_SETUP_H

void configure_SOUND(void);
void configure_LED(void);
=======
#ifndef GPIO_SETUP
#define GPIO_SETUP

void configure_BUZZER();
void configure_LED(char * topico, char * mensagem);
void configure_DHT11();
void configure_HALL();
>>>>>>> 8d538a7ede48e9cc3f5f8277fb08253f0a0a5e6d

#endif