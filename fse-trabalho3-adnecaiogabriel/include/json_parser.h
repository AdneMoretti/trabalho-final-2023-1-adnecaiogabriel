#ifndef JSON_TREATMENT_H
#define JSON_TREATMENT_H

void set_attributes_states(char *key, int value, int topic_id);
void mqtt_event_data_parser(char *data, char *topic);
void send_stop_alarm(char *ALARM_TAG);
void mosquitto_event_data_parser(char *data);
void send_sound(int *sound);

#endif