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
	if (key->ascii != 0) {
		print(key->ascii);
	}
	refresh_screen();
}

/**********************************************
 int_80()
 Atenci—n de interrupci—n de software
 *************************************************/

void int_80(int service_number, int device, char * buffer, int amount) {
	switch (service_number) {
	case READ:
		read(device, buffer, amount);
		break;
	case WRITE:
		write(device, buffer, amount);
		break;
	default:
		break;
	}
}
