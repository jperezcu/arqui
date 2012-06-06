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
		switch (c) {
		case '\n':
			putc(c);
			parse_command();
			shell->cursor = 0;
			break;
		case '\b':
			if (shell->cursor != 0) {
				putc(c);
				shell->cursor--;
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

		char command[11];
		char input[shell->cursor];

		int valid_entry = sscanf(shell->buffer, shell->cursor, &command,
				&input);

		if (valid_entry && streq(command, "echo")) {
			printf("%s\n", input);
		} else if (valid_entry && streq(command, "help")) {
			printf("Pruebe los comandos echo, help, chat, shutdown\n");
		} else {
			printf("Invalid command.\n");
		}
	}

}
