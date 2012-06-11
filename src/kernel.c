#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kernel.h"

DESCR_INT idt[0x100]; /* IDT de 10 entradas */
IDTR idtr; /* IDTR */

extern int received_serial;
extern int received_serial_char;

/**********************************************
 kmain()
 Punto de entrada de c√≥o C.
 *************************************************/

kmain() {

	int i, num;

	/* Prepara el buffer de teclado */
	setup_keyboard_buffer();

	/* Crea las terminales virtuales */
	setup_vts();

	/* Configura el puerto serie */
	setup_serial_port();

	/* Carga de entradas en IDT */
	setup_IDT_content();

	/* Carga de IDTR */
	setup_IDTR();

	_Cli();

	/* Habilita interrupciones en el PIC */
	_mascaraPIC1(KEYBOARD & TIMER_TICK & SERIAL_PORT);
	_mascaraPIC2(0xFF);

	_Sti();

	char *monitor = (char *) 0xb8000;

	monitor[LOWER_SCREEN] = 'p';
	monitor[LOWER_SCREEN + 1] = WHITE_TXT;

	while (1) {
		if (received_serial == TRUE) {
			monitor[LOWER_SCREEN] = received_serial_char;
			monitor[LOWER_SCREEN + 1] = WHITE_TXT;
		}
	}

//	shell_mode();

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
	//	IRQ4: serial port COM1
	setup_IDT_entry(&idt[0x0C], 0x08, (dword) &_int_0C_hand, ACS_INT, 0);
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

struct keyboard_type keyboard = { SPANISH, FALSE, FALSE, FALSE, FALSE, FALSE,
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

struct shell_type shell[VT_AMOUNT] = { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } };

struct vt_type vt[VT_AMOUNT] = { { &screen[0], &shell[0] }, { &screen[1],
		&shell[1] }, { &screen[2], &shell[2] }, { &screen[3], &shell[3] } };

int current_vt = 0;

void setup_vts() {
	int i, j;
	for (i = 0; i < VT_AMOUNT; i++) {
		vt[i].screen->chat_cursor = LOWER_SCREEN;
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

/**********************************************
 setup_serial_port()
 Inicializa los parámetros del puerto serie.
 Código de http://wiki.osdev.org/Serial_Ports
 *************************************************/

char arriving_buffer[CHAT_BUFFER_SIZE];
char departing_buffer[CHAT_BUFFER_SIZE];
int departing_cursor = 0;
int arriving_cursor = 0;

void setup_serial_port() {

	_outb(COM1 + 1, 0x00); // Disable all interrupts
	_outb(COM1 + 3, 0x80); // Enable DLAB (set baud rate divisor)
	_outb(COM1 + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
	_outb(COM1 + 1, 0x00); //                  (hi byte)
	_outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
	_outb(COM1 + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	_outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set

	_outb(0x21, (_inb(0x21) & 0xF7));
	_outb(COM1 + 1, 0x01);

}

void change_terminal(int number) {
	if (current_vt != number) {
		current_vt = number;
		refresh_screen();
	}
}
