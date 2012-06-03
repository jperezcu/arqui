#include "../include/defs.h"

extern int tickpos;

/**********************************************
 int_08()
 Atenci—n de interrupci—n del timer tick
 *************************************************/

void int_08() {
//	char *video = (char *) 0xb8000;
//	video[tickpos += 2] = 'o';
}

/**********************************************
 int_09()
 Atenci—n de interrupci—n del keyboard
 *************************************************/

void int_09(unsigned char scancode) {
	struct key_type * key = (struct key_type *) parse_scancode(scancode);
	if (key->kind == ALPHANUM_KEY) {
		print(key->ascii);
	}
	switch (key->ascii) {
	case '\b':
		del();
		break;
	case '\n':
		skip_line();
		break;
	default:
		break;
	}
	refresh_screen();
}
