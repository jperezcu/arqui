#include "../include/defs.h"
#include "../include/shell.h"

extern int current_vt;
extern vt_type vt[];

void start_shell() {
	char c;

	while (1) {
		while ((c = getc()) == -1) {
			;
		}
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
			add_to_vt[current_vt].shell_buffer(c);
			break;
		}
		refresh_screen();
	}
}

void add_to_shell_buffer(unsigned char c) {

	vt[current_vt].shell->buffer[vt[current_vt].shell->cursor] = c;

	if (vt[current_vt].shell->cursor == vt[current_vt].shell_BUFFER_SIZE - 1) {
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

void chat_mode() {



	//habilita interrupcion de puerto serie
	_mascaraPIC1(0xE8);



	//deshabilita interrupcion de puerto serie
	_mascaraPIC1(0xF8);
}
