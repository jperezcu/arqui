#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0xA]; /* IDT de 10 entradas */
IDTR idtr; /* IDTR */

struct keyboard_type keyboard;
int current_vt;
vt_type vt[4];

/* Definirá las entradas del IDT */
void setup_IDT_content();

/* Definirá el IDTR */
void setup_IDTR();

/* Definirá el teclado */
void setup_keyboard();

/* Definirá las terminales virtuales */
void setup_vts();

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
	setup_vts();

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
 Inicializa el teclado.
 *************************************************/

void setup_keyboard() {
	keyboard.language = ENGLISH;
	keyboard.caps_state = FALSE;
	keyboard.shift_state = FALSE;
	keyboard.dead_key = FALSE;
}

/**********************************************
 setup_vts()
 Inicializa las terminales virtuales.
 *************************************************/

void setup_vts() {
	current_vt = 0;
	int i;

	screen_type screen;
	for (i = 0; i < SCREEN_SIZE; i++) {
		screen.content[i++] = 0;
		screen.content[i] = WHITE_TXT;
	}
	screen.cursor = 0;

	input_type input;
	for (i = 0; i < INPUT_BUFFER_SIZE; i++) {
		input.buffer[i++] = 0;
		input.buffer[i] = WHITE_TXT;
	}
	input.cursor = 0;

	vt_type term = { &screen, &input };
	vt[0] = term;
}

void deb(unsigned char c) {

	char *monitor = (char *) 0xb8000;
	monitor[1] = WHITE_TXT;
	monitor[0] = c;
}
