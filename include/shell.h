#ifndef _SHELL_H
#define _SHELL_H

/**********************************
 *
 *
 ***********************************/

void shell_mode();
void print_initial_prompt_lines();
void add_to_shell_buffer(unsigned char c);
void parse_command();
void chat_mode();
void print_on_main_screen(int cursor);
void send_departing_buffer(int cursor);
int serial_received();
char read_serial();
int is_transmit_empty();
void parse_arriving_char(char c);
void parse_departing_char(char c);
void parse_command_char(char c);


#endif
