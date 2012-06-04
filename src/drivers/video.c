#include "../../include/defs.h"
#include "../../include/video.h"

extern int current_vt;
extern struct vt_type vt[];

void print(char c) {

	screen_type * screen = vt[current_vt].screen;
	input_type * input = vt[current_vt].input;

	if (input->cursor == INPUT_BUFFER_SIZE - 1) {
		input->cursor = 0;
	}
	if (screen->cursor == SCREEN_SIZE) {
		move_screen();
	}

	input->buffer[input->cursor++] = c;
	input->buffer[input->cursor++] = WHITE_TXT;
	screen->content[screen->cursor++] = c;
	screen->content[screen->cursor++] = WHITE_TXT;
}

void del() {
//	TODO escribo primer caracter, borro y no reemplaza el primer lugar por caracter vacio.

	screen_type * screen = vt[current_vt].screen;
	input_type * input = vt[current_vt].input;

	if (screen->cursor != 0 && input->cursor != 0) {
		input->cursor -= 2;

		screen->content[screen->cursor - 2] = ' ';
		screen->content[screen->cursor - 1] = WHITE_TXT;

		screen->cursor -= 2;
	}

}
void skip_line() {

	screen_type * screen = vt[current_vt].screen;
	input_type * input = vt[current_vt].input;

	input->cursor = 0;
	if ((screen->cursor >= LAST_LINE_BEGIN)
			&& (screen->cursor <= LAST_LINE_END)) {
		move_screen();
	} else {
		screen->cursor += WIDTH * 2 - (screen->cursor % (WIDTH * 2));
	}
}

void refresh_screen() {

	screen_type * screen = vt[current_vt].screen;

	char *monitor = (char *) 0xb8000;
	int i;
	for (i = 0; i < (screen->cursor * 2); i++) {
		monitor[i] = screen->content[i];
	}
}

void move_screen() {

	screen_type * screen = vt[current_vt].screen;
	input_type * input = vt[current_vt].input;

	int i, j;
	for (i = 0, j = (WIDTH * 2); j < SCREEN_SIZE; i++, j++) {
		screen->content[i] = screen->content[j];
	}
	while (i < SCREEN_SIZE) {
		screen->content[i++] = ' ';
		screen->content[i++] = WHITE_TXT;
	}

	screen->cursor = LAST_LINE_BEGIN;
}
