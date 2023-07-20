#ifndef JSON_TREATMENT_H
#define JSON_TREATMENT_H

void mqtt_event_data_parser(char *data, char *topic);
void send_stop_alarm(char *ALARM_TAG);
void mosquitto_event_data_parser(char *data);
void send_sound(int *sound);
void send_magnetic_signal(int magnetic_signal);
void send_temperature_telemetry();
void send_dashboard_signal(int value);

#endif