#include "../include/kc.h"

extern vt_type * vt;
extern int current_vt;

/***************************************************************
 *k_clear_screen
 *
 * Borra la pantalla en modo texto color.
 ****************************************************************/

void k_clear_screen() {
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	while (i < (80 * 25 * 2)) {
		vidmem[i] = ' ';
		i++;
		vidmem[i] = WHITE_TXT;
		i++;
	};
}

/***************************************************************
 *setup_IDT_entry
 * Inicializa un descriptor de la IDT
 *
 *Recibe: Puntero a elemento de la IDT
 *	 Selector a cargar en el descriptor de interrupcion
 *	 Puntero a rutina de atencion de interrupcion
 *	 Derechos de acceso del segmento
 *	 Cero
 ****************************************************************/

void setup_IDT_entry(DESCR_INT *item, byte selector, dword offset, byte access,
		byte cero) {
	item->selector = selector;
	item->offset_l = offset & 0xFFFF;
	item->offset_h = offset >> 16;
	item->access = access;
	item->cero = cero;
}

void putc(unsigned char c) {
	char c_copy = c;
	_system_call(WRITE, STDOUT, &c_copy, 1);
}

unsigned char getc() {
	char c;
//	_system_call(READ, STDIN, &c_copy, 1);
	return c;
}

void write(int device, char * buffer, int amount) {

	if (device == STDOUT) {
		int i;
		for (i = 0; i < amount; i++) {
			print(buffer[i]);
		}
	}
}

void read(int device, char * buffer, int amount) {
	if (device == STDIN) {

	}
}
