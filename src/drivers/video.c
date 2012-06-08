#include "../../include/defs.h"
#include "../../include/video.h"

extern int current_vt;
extern struct vt_type vt[];
int main_screen_size = SCREEN_SIZE;

void print(char c) {

	screen_type * screen = vt[current_vt].screen;

	switch (c) {
	case '\b':
		del();
		break;
	case '\n':
		skip_line();
		break;
	default:
		if (screen->cursor == main_screen_size) {
			move_screen();
		}

		screen->content[screen->cursor++] = c;
		screen->content[screen->cursor++] = WHITE_TXT;
		break;
	}
}

void change_main_screen_size(int i) {
	main_screen_size -= i;
}

void print_lower_screen(char c) {
	int cursor = main_screen_size + 1;

	switch (c) {
		case '\b':
			del();
			break;
		default:
			if (screen->cursor == main_screen_size) {
				move_screen();
			}

			screen->content[screen->cursor++] = c;
			screen->content[screen->cursor++] = WHITE_TXT;
			break;

}

void del() {

	screen_type * screen = vt[current_vt].screen;

	if (screen->cursor != 0) {

		screen->content[screen->cursor - 2] = ' ';
		screen->content[screen->cursor - 1] = WHITE_TXT;

		screen->cursor -= 2;
	}

}
void skip_line() {

	screen_type * screen = vt[current_vt].screen;

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

	monitor[SCREEN_SIZE - 2] = current_vt + '0';
	monitor[SCREEN_SIZE - 1] = WHITE_TXT;

	int i;
	for (i = 0; i < SCREEN_SIZE - 2; i++) {
		monitor[i] = screen->content[i];
	}
}

void move_screen() {

	screen_type * screen = vt[current_vt].screen;

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
