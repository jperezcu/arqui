#include "../include/defs.h"
#include "../include/shell.h"

extern int current_vt;
extern vt_type vt[];
extern char departing_buffer[CHAT_BUFFER_SIZE];
extern char arriving_buffer[CHAT_BUFFER_SIZE];
extern int departing_cursor;
extern int arriving_cursor;

extern int received_serial;
extern char received_serial_char;

void shell_mode() {

	char arriving_char;
	char c;

	print_initial_prompt_lines();

	while (1) {

		if (current_vt == CHAT_VT && received_serial) {
			parse_arriving_char(received_serial_char);
		}

		if ((c = getc()) != -1) {

			if (current_vt == CHAT_VT) {
				parse_departing_char(c);
			} else {
				parse_command_char(c);
			}
		}

		refresh_screen();
	}
}

void print_initial_prompt_lines() {
	printf("Bienvenido a J2OS!\n");
	for (; current_vt < 3; current_vt++) {
		printf("guest@J2OS-terminal-%d:/$ ", current_vt + 1);
	}
	current_vt = 0;
	refresh_screen();
}

void print_on_main_screen(char * buffer, int cursor) {
	int i;
	int lines;
	int aux;
	aux = vt[current_vt].screen->cursor + (lines * WIDTH * 2);
	if (vt[current_vt].screen->cursor >= LOWER_SCREEN) {
			move_screen(LOWER_SCREEN);
			vt[current_vt].screen->cursor = LOWER_SCREEN - (2 * WIDTH);
	}
	for (i = 0; i < cursor; i++) {
		putc(buffer[i]);
	}
	putc('\n');

}

int serial_received() {
	return _inb(COM1 + 5) & 1;
}

char read_serial() {
	return _inb(COM1);
}

void parse_arriving_char(char arriving_char) {
	if ((arriving_char == 12) || (arriving_char == 13)
			|| (arriving_char == '\n')) {
		print_on_main_screen(arriving_buffer, arriving_cursor);
		arriving_cursor = 0;
	} else if(arriving_char =='\b'){
		
			if (arriving_cursor != 0) {
				arriving_cursor--;
			}
		} else {
			
				arriving_buffer[arriving_cursor] = arriving_char;
				arriving_cursor++;
		}
	received_serial = FALSE;
}

int is_transmit_empty() {
	return _inb(COM1 + 5) & 0x20;
}

/*void parse_departing_char(char c) {
	switch (c) {
	case '\n':
		print_on_main_screen(departing_buffer, departing_cursor);
		clear_lower_screen();
		departing_buffer[departing_cursor++] = c;
		send_departing_buffer(departing_cursor);
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
}*/

void parse_departing_char(char c) {
	switch (c) {
	case '\n':
		print_on_main_screen(departing_buffer, departing_cursor);
		clear_lower_screen();
		//departing_buffer[departing_cursor++] = c;
		//send_departing_char(c);
		departing_cursor = 0;
		break;
	case '\b':
		if (departing_cursor != 0) {
			putc_lower_screen(c);
			departing_cursor--;
		}
		break;
	default:
		 {
			 if(departing_cursor<CHAT_BUFFER_SIZE){
				putc_lower_screen(c);
				departing_buffer[departing_cursor] = c;
				departing_cursor++;
			}
		}
		break;
	}
	if(departing_cursor<CHAT_BUFFER_SIZE){
		send_departing_char(c);
	}
}

/*void send_departing_buffer(int cursor) {

	int i = 0;

	//_Cli();
	for (i = 0; i < cursor;) {
		if (is_transmit_empty()) {
			//printf("transmit empty TRUE\n");
			_outb(COM1, departing_buffer[i]); //no quiero ser interrumpido mientras escribo en el P.S
			i++;
		}
	}
	//_Sti();
}*/

void send_departing_char(char departing_char) {

	//int i = 0;

	_Cli();
	//for (i = 0; i < cursor;) {
		//if (is_transmit_empty()) {
			//printf("transmit empty TRUE\n");
			_outb(COM1, departing_char); //no quiero ser interrumpido mientras escribo en el P.S
			//i++;
		//}
	//}
	_Sti();
}

void parse_command_char(char c) {
	switch (c) {
	case '\n':
		putc(c);
		parse_command();
		vt[current_vt].shell->cursor = 0;
		printf("guest@J2OS-terminal-%d:/$ ", current_vt + 1);
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
			printf("\nPruebe los siguientes comandos:\n\n");
			printf(
					"echo <mensaje> - imprime el mensaje en la terminal actual.\n\n");
			printf("clear - borra el contenido de la terminal actual.\n\n");
			printf(
					"Cambie de terminal presionando ALT + 1 (o ALT + 2 o ALT + 3). \n\n");
			printf(
					"Entre al modo chat presionando ALT + 4.\n\n");
		} else if (valid_entry && streq(command, "clear")) {
			clear();
		} else {
			printf("Invalid command. Please use the command 'help' for more information.\n");
		}
	}

}

