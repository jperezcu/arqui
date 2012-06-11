#include "../include/defs.h"
#include "../include/shell.h"

extern int current_vt;
extern vt_type vt[];
extern char departing_buffer[CHAT_BUFFER_SIZE];
extern char arriving_buffer[CHAT_BUFFER_SIZE];
extern int departing_cursor;
extern int arriving_cursor;

void shell_mode() {

	char arriving_char;
	char c;

	print_initial_prompt_lines();

	while (1) {

		if (current_vt == CHAT_VT && serial_received()) {
			arriving_char = read_serial();
			parse_arriving_char(arriving_char);
		}

		while ((c = getc()) == -1)
			;

		if (current_vt == CHAT_VT) {
			parse_departing_char(c);
		} else {
			parse_command_char(c);
		}

		refresh_screen();
	}
}

void print_initial_prompt_lines() {
	for (; current_vt < 3; current_vt++) {
		printf("guest@J2OS-terminal-%d:/$ ", current_vt + 1);
	}
	current_vt = 0;
	refresh_screen();
}

void print_on_main_screen(int cursor) {
	int i;
	int lines;
	int aux;
	lines = (cursor / WIDTH) + 1;
	aux = vt[current_vt].screen->cursor + (lines * WIDTH * 2);
	if (aux >= LOWER_SCREEN) {
		aux = ((aux - LOWER_SCREEN) / (2 * WIDTH));
		printf("%d", aux);

	}
	if (vt[current_vt].screen->cursor >= LOWER_SCREEN) {
		for (i = 0; i < aux; i++) {
			move_screen(LOWER_SCREEN);
		}
		vt[current_vt].screen->cursor = LOWER_SCREEN - (2 * WIDTH * lines);
	}
	for (i = 0; i < cursor; i++) {
		putc(departing_buffer[i]);
	}
	putc('\n');

}

int serial_received() {
	return _inb(SERIAL_PORT + 5) & 1;
}

char read_serial() {
	return _inb(SERIAL_PORT);
}

void parse_arriving_char(char arriving_char) {
	if (arriving_char == '\n') {
		print_on_main_screen(arriving_cursor);
		arriving_cursor = 0;
	} else {
		arriving_buffer[arriving_cursor] = arriving_char;
		arriving_cursor++;
	}
}

int is_transmit_empty() {
	return _inb(SERIAL_PORT + 5) & 0x20;
}

void parse_departing_char(char c) {
	switch (c) {
	case '\n':
		print_on_main_screen(departing_cursor);
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
}

void send_departing_buffer(int cursor) {

	while (is_transmit_empty() == 0)
		;

	int i = 0;
	for (i = 0; i < cursor; i++) {
		_Cli();
		_outb(SERIAL_PORT, departing_buffer[i]); //no quiero ser interrumpido mientras escribo en el P.S
		_Sti();
	}
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
			printf("Pruebe los comandos echo, help, chat, shutdown\n");
		} else {
			printf("Invalid command.\n");
		}
	}

}

