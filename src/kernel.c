#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0x100]; /* IDT de 10 entradas */
IDTR idtr; /* IDTR */

/* Definirá las entradas del IDT */
void setup_IDT_content();

/* Definirá el IDTR */
void setup_IDTR();

/* Definirá las terminales virtuales */
void setup_vts();

void setup_keyboard_buffer();

/**********************************************
 kmain()
 Punto de entrada de c√≥o C.
 *************************************************/

kmain() {

	int i, num;

	/* Borra la pantalla. */
	k_clear_screen();

	setup_keyboard_buffer();

	/* Crea la pantalla */
	setup_vts();

	/* Carga de entradas en IDT */
	setup_IDT_content();

	/* Carga de IDTR */
	setup_IDTR();

	_Cli();

	/* Habilita interrupciones en el PIC */
	_mascaraPIC1(0xF8);
	_mascaraPIC2(0xFF);

	_Sti();

//	for (i = 0; i < SCREEN_SIZE - 2; i++) {
//		putc('a');
//	}
	start_shell();
	//printf("Esto es un nu%mero: %d \nEsto es un c\aracter: %c \nEsto es un string: %s \n",34, "T","hello world");
	//refresh_screen();

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
	//	IRQ2: int80
	setup_IDT_entry(&idt[0x80], 0x08, (dword) &_int_80_hand, ACS_INT, 0);
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
 Inicializa el teclado.
 *************************************************/

struct keyboard_type keyboard = { ENGLISH, FALSE, FALSE, FALSE, FALSE, FALSE,
		FALSE, 0, 0 };

void setup_keyboard_buffer() {
	int j;
	for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++) {
		keyboard.buffer[j] = 0;
	}
}

/**********************************************
 setup_vts()
 Inicializa las terminales virtuales.
 *************************************************/

struct screen_type screen[VT_AMOUNT] = { { 0 }, { 0 }, { 0 }, { 0 } };

struct shell_type shell[VT_AMOUNT] = { { 0 }, { 0 }, { 0 }, { 0 } };

struct vt_type vt[VT_AMOUNT] = { { &screen[0], &shell[0] }, { &screen[1],
		&shell[1] }, { &screen[2], &shell[2] }, { &screen[3], &shell[3] } };

int current_vt = 0;

void setup_vts() {
	int i, j;
	for (i = 0; i < VT_AMOUNT; i++) {
		for (j = 0; j < SCREEN_SIZE; j++) {
			vt[i].screen->content[j++] = 0;
			vt[i].screen->content[j] = WHITE_TXT;
		}
		for (j = 0; j < SHELL_BUFFER_SIZE; j++) {
			vt[i].shell->buffer[j] = WHITE_TXT;
			vt[i].shell->buffer[j++] = 0;
		}
	}

}

void change_terminal(int number) {
	if (current_vt != number) {
		current_vt = number;
		refresh_screen();
	}
}

void deb(unsigned char c) {

	char *monitor = (char *) 0xb8000;
	monitor[SCREEN_SIZE - WIDTH * 2] = WHITE_TXT;
	monitor[SCREEN_SIZE - WIDTH * 2 + 1] = c + '0';
}
