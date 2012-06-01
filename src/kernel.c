#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0xA]; /* IDT de 10 entradas */
IDTR idtr; /* IDTR */

int tickpos = 0;

/* Definir‡ las entradas del IDT */
void setup_IDT_content();

/* Definir‡ el IDTR */
void setup_IDTR();

/**********************************************
 kmain()
 Punto de entrada de cÃ³o C.
 *************************************************/

kmain() {

	int i, num;

	/* Borra la pantalla. */
	k_clear_screen();

	/* Carga de entradas en IDT */
	setup_IDT_content();

	/* Carga de IDTR */
	setup_IDTR();

	_Cli();

	/* Habilita interrupciones en el PIC */
	_mascaraPIC1(0xFC);
	_mascaraPIC2(0xFF);

	_Sti();

	while (1) {
	}
}

/**********************************************
 setup_IDT_content()
 Carga de IDT con rutinas de atenci—n IRQ
 *************************************************/

void setup_IDT_content() {
	//	IRQ0: timer tick
	setup_IDT_entry(&idt[0x08], 0x08, (dword) &_int_08_hand, ACS_INT, 0);
	//	IRQ1: keyboard
	setup_IDT_entry(&idt[0x09], 0x08, (dword) &_int_09_hand, ACS_INT, 0);
}

/**********************************************
 setup_IDTR()
 Carga de IDTR
 *************************************************/

void setup_IDTR() {
	idtr.base = 0;
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt) - 1;

	_lidt(&idtr);
}

/**********************************************
 int_08()
 Atenci—n de interrupci—n del timer tick
 *************************************************/

void int_08() {
	char *video = (char *) 0xb8000;
	video[tickpos += 2] = 'o';
}

/**********************************************
 int_09()
 Atenci—n de interrupci—n del keyboard
 *************************************************/

void int_09(unsigned char scancode) {
	struct key_type * key = (struct key_type *) parse_scancode(scancode);
	if (key->kind == ALPHANUM_KEY) {
		char *video = (char *) 0xb8000;
		video[tickpos += 2] = key->ascii;
	}

}
