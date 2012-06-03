#include "../../include/defs.h"
#include "../../include/kc.h"

extern screen_type screen;

void refresh_screen();
void move_screen();

void print(char c) {
	if (screen.line_pos == LINE_BUFFER_SIZE - 1) {
		screen.line_pos = 0;
	}
	if (screen.video_pos == WIDTH * HEIGHT * 2) {
		move_screen();
	}
	screen.line_buffer[screen.line_pos++] = c;
	screen.line_buffer[screen.line_pos++] = WHITE_TXT;

	screen.video[screen.video_pos++] = c;
	screen.video[screen.video_pos++] = WHITE_TXT;

}

void del() {
//	TODO escribo primer caracter, borro y no reemplaza el primer lugar por caracter vacio.

	if (screen.video_pos != 0 && screen.line_pos != 0) {
		screen.line_pos -= 2;

//		esta bien para vaciar caracter??
		screen.video[screen.video_pos - 2] = ' ';
		screen.video[screen.video_pos - 1] = WHITE_TXT;

		screen.video_pos -= 2;
	}

}
void skip_line() {
	screen.line_pos = 0;
	if ((screen.video_pos >= WIDTH * (HEIGHT - 1) * 2)
			&& (screen.video_pos < (WIDTH * HEIGHT * 2))) {
		move_screen();
	} else {
		screen.video_pos += WIDTH * 2 - (screen.video_pos % (WIDTH * 2));
	}
}

void refresh_screen() {
	char *monitor = (char *) 0xb8000;
	int i;
	for (i = 0; i < (screen.video_pos * 2); i++) {
		monitor[i] = screen.video[i];
	}
}

void move_screen() {
	int i, j;
	for (i = 0, j = (WIDTH * 2); j < (2 * WIDTH * HEIGHT); i++, j++) {
		screen.video[i] = screen.video[j];
	}
	while (i < (2 * WIDTH * HEIGHT)) {
		screen.video[i++] = ' ';
		screen.video[i++] = WHITE_TXT;
	}

	screen.video_pos = WIDTH * (HEIGHT - 1) * 2;
}
