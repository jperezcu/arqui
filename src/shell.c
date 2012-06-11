#include "../include/defs.h"
#include "../include/shell.h"

extern int current_vt;
extern vt_type vt[];
extern char departing_buffer[CHAT_BUFFER_SIZE];
extern char arriving_buffer[CHAT_BUFFER_SIZE];

int received_serial = FALSE;
int received_serial_char;

void start_shell() {
	char c;
	while (1) {
		while ((c = getc()) == -1)
			;
		switch (c) {
		case '\n':
			putc(c);
			parse_command();
			vt[current_vt].shell->cursor = 0;
			break;
		case '\b':
			if (vt[current_vt].shell->cursor != 0) {
				putc(c);
				vt[current_vt].shell->cursor--;
			}
			break;
		default:
			putc(c);
			add_to_shell_buffer(c);
			break;
		}
		refresh_screen();
	}
}

void add_to_shell_buffer(unsigned char c) {

	vt[current_vt].shell->buffer[vt[current_vt].shell->cursor] = c;

	if (vt[current_vt].shell->cursor == SHELL_BUFFER_SIZE - 1) {
		vt[current_vt].shell->cursor = 0;
	} else {
		vt[current_vt].shell->cursor++;
	}

}

void parse_command() {
	if (vt[current_vt].shell->cursor != 0) {

		char command[11];
		char input[vt[current_vt].shell->cursor];

		int valid_entry = sscanf(vt[current_vt].shell->buffer,
				vt[current_vt].shell->cursor, &command, &input);

		if (valid_entry && streq(command, "echo")) {
			printf("%s\n", input);
		} else if (valid_entry && streq(command, "help")) {
			printf("Pruebe los comandos echo, help, chat, shutdown\n");
//		} else if (valid_entry && streq(command, "chat")) {
//			k_clear_screen();
//			int i;
//			for (i = 0; i < SCREEN_SIZE; i++) {
//				vt[current_vt].screen->content[i++] = ' ';
//				vt[current_vt].screen->content[i] = WHITE_TXT; // hacer clear screen
//			}
//			vt[current_vt].screen->cursor = 0;
//			refresh_screen();
//			chat_mode();
		} else {
			printf("Invalid command.\n");
		}
	}

}

void chat_mode() {

	//habilita interrupcion de puerto serie
	//_mascaraPIC1(0xE8);

	int departing_cursor = 0;
	int arriving_cursor = 0;
	char c;

	while (1) {
		if (received_serial == TRUE) {
			if (received_serial_char == '\n') {
				print_on_main_screen(arriving_cursor);
				arriving_cursor = 0;
			} else {
				arriving_buffer[arriving_cursor] = received_serial_char;
				arriving_cursor++;
			}
		}
		c = getc();
		if (c != -1) {
			printf("getc del chat anduvo");
			switch (c) {
			case '\n':
				clear_lower_screen();
				print_on_main_screen(departing_cursor);
				//send_departing_buffer(departing_cursor);
				departing_cursor = 0;
				break;
			case '\b':
				if (departing_cursor != 0) {
					putc_lower_screen(c);
					departing_cursor--;
				}
				break;
			default:
				if (departing_cursor < CHAT_BUFFER_SIZE) {
					putc_lower_screen(c);
					departing_buffer[departing_cursor] = c;
					departing_cursor++;
				}
				break;
			}
		}
		refresh_screen();
	}

//deshabilita interrupcion de puerto serie

	//_mascaraPIC1(0xF8);

}

void print_on_main_screen(int cursor) {
	int i;
	for (i = 0; i < cursor; i++) {
		putc(departing_buffer[i]);
	}
	putc('\n');
}

void send_departing_buffer(int cursor) {
	int i = 0;
	for (i = 0; i < cursor; i++) {
		_Cli();
		_outb(SERIAL_PORT, departing_buffer[i]); //no quiero ser interrumpido mientras escribo en el P.S
		_Sti();
	}
}

