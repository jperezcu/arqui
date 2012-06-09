#ifndef _VIDEO_H
#define _VIDEO_H

/**********************************
*
*
***********************************/

void print(char c);
void del();
void refresh_screen();
void move_screen(int screen_size);
void skip_line();
void print_lower_screen(char c);
void clear_lower_screen();
void change_main_screen_size(int i);

#endif
