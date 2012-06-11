#include "../include/defs.h"

extern int tickpos;
int received_serial = FALSE;
int received_serial_char;

/**********************************************
 int_08()
 Atención de interrupción del timer tick
 *************************************************/

void int_08() {
//	char *video = (char *) 0xb8000;
//	video[tickpos += 2] = 'o';
}

/**********************************************
 int_09()
 Atención de interrupción del keyboard
 *************************************************/

void int_09(unsigned char scancode) {

	struct key_type * key = (struct key_type *) parse_scancode(scancode);
	if (key->kind != HIDDEN_KEY) {
		add_to_keyboard_buffer(key->ascii);
	}
}

/**********************************************
 int_80()
 Atención de interrupción de software
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

/**********************************************
 int_0C()
 Atenci—n de puerto serie
 *************************************************/

void int_0C(char c) {

	_Cli();
	received_serial_char = c;
	_Sti();
	received_serial = TRUE;
}
