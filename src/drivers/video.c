#include "../../include/defs.h"
#include "../../include/kc.h"

extern screen_type screen;

void refresh_screen();

void print(char c) {
	if (screen.line_pos == LINE_BUFFER_SIZE - 1) {
		screen.line_pos = 0;
	}
	screen.line_buffer[screen.line_pos++] = c;
	screen.line_buffer[screen.line_pos++] = WHITE_TXT;

	screen.video[screen.video_pos++] = c;
	screen.video[screen.video_pos++] = WHITE_TXT;

	refresh_screen();
}

void del() {

	if (screen.video_pos != 0 && screen.line_pos != 0) {
		screen.line_pos-=2;

		screen.video[screen.video_pos-2] = ' ';
		screen.video[screen.video_pos-1] = WHITE_TXT;

		refresh_screen();

		screen.video_pos-=2;
	}
}

void refresh_screen() {
	char *monitor  = (char *) 0xb8000;
	int i;
	for (i = 0; i < screen.video_pos; i++) {
		monitor[i++] = screen.video[i];
		monitor[i] = screen.video[i];
	}
}
