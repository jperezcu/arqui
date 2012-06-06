#ifndef _KEYBOARD_H
#define _KEYBOARD_H

/**********************************
 *
 *
 ***********************************/

struct key_type * parse_scancode(unsigned char c);
int printable(unsigned char c);
int is_letter(unsigned char c);
int is_vowel(unsigned char c);
int is_terminal_number(unsigned char c);
unsigned char get_char_from_keyboard_buffer();
int keyboard_buffer_can_read();
void add_to_keyboard_buffer(unsigned char c);

#endif
