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

#endif
