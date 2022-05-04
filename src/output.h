#ifndef OUTPUT_H
#define OUPUT_H


void update_outputs();
void turn_on (uint8_t out);
void turn_off(uint8_t out);
void toggle_output(uint8_t out);
void on_Timed(uint8_t out, int time);
void init_outputs();
void update_output();

#endif