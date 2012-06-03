#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0xA]; /* IDT de 10 entradas */
IDTR idtr; /* IDTR */

int tickpos = -2;

struct keyboard_type keyboard;
struct screen_type screen;

/* Definirá las entradas del IDT */
void setup_IDT_content();

/* Definirá el IDTR */
void setup_IDTR();

/* Definirá el teclado */
void setup_keyboard();

/* Definirá la pantalla */
void setup_screen();

/**********************************************
 kmain()
 Punto de entrada de c√≥o C.
 *************************************************/

kmain() {

	int i, num;

	/* Borra la pantalla. */
	k_clear_screen();

	/* Crea el teclado default */
	setup_keyboard();

	/* Crea la pantalla*/
		setup_screen();

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
 Carga de IDT con rutinas de atención IRQ
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
 setup_keyboard()
 Inicializa el keyboard.
 *************************************************/

void setup_keyboard() {
	keyboard.language = ENGLISH;
	keyboard.state = LOWER;
	keyboard.caps_state = FALSE;
	keyboard.shift_state = FALSE;
}

/**********************************************
 setup_screen()
 Inicializa la pantalla.
 *************************************************/

void setup_screen() {
	screen.line_pos = 0;
	screen.video_pos = 0;
}
