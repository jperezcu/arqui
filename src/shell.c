#include "../include/defs.h"
#include "../include/shell.h"

extern int current_vt;
extern vt_type vt[];

void start_shell() {
	shell_type * shell = vt[current_vt].shell;
	char c;

	while (1) {
		while ((c = getc()) == -1) {
			;
		}
		putc(c);
		if (c == '\n') {
			parse_command();
			shell->cursor = 0;
		} else {
			add_to_shell_buffer(c);
		}
		refresh_screen();
	}
}

void add_to_shell_buffer(unsigned char c) {

	shell_type * shell = vt[current_vt].shell;

	shell->buffer[shell->cursor] = c;

	if (shell->cursor == SHELL_BUFFER_SIZE - 1) {
		shell->cursor = 0;
	} else {
		shell->cursor++;
	}

}

void parse_command() {

	shell_type * shell = vt[current_vt].shell;

	if (shell->cursor != 0) {
		switch (shell->buffer[0]) {
		case 'h':
			printf("Help\n");
			break;
		default:
			printf("Error\n");
		}
	}
}
