#ifndef JSON_TREATMENT_H
#define JSON_TREATMENT_H

void set_attributes_states(char *key, int value, int topic_id);
void mqtt_event_data_parser(char *data, char *topic);

#endif