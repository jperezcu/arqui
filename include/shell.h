#ifndef _SHELL_H
#define _SHELL_H

/**********************************
 *
 *
 ***********************************/

void start_shell();
void add_to_shell_buffer(unsigned char c);
void parse_command();
void chat_mode();
void print_on_main_screen(int cursor);
void send_departing_buffer(int cursor);

#endif
