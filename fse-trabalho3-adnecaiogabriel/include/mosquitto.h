#ifndef MOSQUITTO_H
#define MOSQUITTO_H

void mosquitto_start();
void mosquitto_envia_mensagem(char * topico, char * mensagem);

#endif