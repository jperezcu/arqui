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
			move_screen(main_screen_size);
		}

		screen->content[screen->cursor++] = c;
		screen->content[screen->cursor++] = WHITE_TXT;
		break;
	}
}

void change_main_screen_size(int i) {
	main_screen_size -= i;
}

//problemas con el cursor
void del() {

	screen_type * screen = vt[current_vt].screen;

	if (screen->cursor != 0) {

		screen->content[screen->cursor - 2] = ' ';
		screen->content[screen->cursor - 1] = WHITE_TXT;

		screen->cursor -= 2;
	}

}
void del_lower_screen() {

	screen_type * screen = vt[current_vt].screen;
	if (screen->chat_cursor != LOWER_SCREEN) {

		screen->content[screen->chat_cursor - 2] = ' ';
		screen->content[screen->chat_cursor - 1] = WHITE_TXT;

		screen->chat_cursor -= 2;
	}

}
void skip_line() {

	screen_type * screen = vt[current_vt].screen;

	if ((screen->cursor >= LAST_LINE_BEGIN)
			&& (screen->cursor <= LAST_LINE_END)) {
		move_screen(SCREEN_SIZE);
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
		monitor[i] = vt[current_vt].screen->content[i];
	}

	int position = (current_vt == CHAT_VT) ? screen->chat_cursor : screen->cursor;

	update_cursor(position/2);
}

void move_screen(int screen_size) {

	screen_type * screen = vt[current_vt].screen;

	int i, j;
	for (i = 0, j = (WIDTH * 2); j < screen_size; i++, j++) {
		screen->content[i] = screen->content[j];
	}
	while (i < SCREEN_SIZE) { 
		screen->content[i++] = ' ';
		screen->content[i++] = WHITE_TXT;
	}

	screen->cursor = (screen_size - (WIDTH * 2));
}

void clear() {
	int i = 0;
	screen_type * screen = vt[current_vt].screen;
	for (i = 0; i < screen->cursor;) {
		screen->content[i++] = ' ';
		screen->content[i++] = WHITE_TXT;
	}
	vt[current_vt].screen->cursor = 0;
}

void clear_lower_screen() {
	int i = 0;
	screen_type * screen = vt[current_vt].screen;
	for (i = LOWER_SCREEN; i < screen->chat_cursor;) {
		screen->content[i++] = ' ';
		screen->content[i++] = WHITE_TXT;
	}
	vt[current_vt].screen->chat_cursor = LOWER_SCREEN;
}

void print_lower_screen(char c) {

	screen_type * screen = vt[current_vt].screen;

	switch (c) {
	case '\b':
		del_lower_screen();
		break;
	default:
		screen->content[screen->chat_cursor++] = c;
		screen->content[screen->chat_cursor++] = WHITE_TXT;
		break;
	}
	if (screen->chat_cursor == CHAT_BUFFER_SIZE) {
		screen->chat_cursor = LOWER_SCREEN;
	}

}

void update_cursor(int position) {

	// parte baja del puerto del cursor
	_outb(0x3D4, 0x0F);
	_outb(0x3D5, (unsigned char) (position & 0xFF));

	// parte alta del puerto del cursor
	_outb(0x3D4, 0x0E);
	_outb(0x3D5, (unsigned char) ((position >> 8) & 0xFF));
}
