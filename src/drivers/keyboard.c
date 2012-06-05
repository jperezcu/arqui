#include "../../include/defs.h"
#include "../../include/keyboard.h"

struct key_type * key;
extern keyboard_type keyboard;
extern int current_vt;
extern vt_type vt[];

unsigned char scancode_table[LANGUAGES][STATES][KEYS] = { { { 0, 0x1b, '1', '2',
		'3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w',
		'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
		'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\\', 0, '\\', 'z', 'x',
		'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\\', '.', 56, ' ', 58, 59, 60,
		61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
		79, 80, 81, 82, 83, 84, 85, '<' },

{ 0, 0x1b, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
		'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0xb3, 0, '|',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\\', '.', 56, ' ',
		58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
		76, 77, 78, 79, 80, 81, 82, 83, 84, 85, '>' } }, { { 0, 0xb3, '1', '2',
		'3', '4', '5', '6', '7', '8', '9', '0', '\'', 0xa8, '\b', '\t', 'q',
		'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '´', '+', '\n', 0, 'a',
		's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0xa4, '{', 0xb3, 0, '}', 'z',
		'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '\\', '.', 56, ' ', 58, 59,
		60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77,
		78, 79, 80, 81, 82, 83, 84, 85, '<' },

{ 0, 0xa7, '!', '"', '#', '$', '%', '&', '/', '(', ')', '=', '?', 0xad, '\b',
		'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '¨', '*', '\n',
		0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0xa5, '[', 0xa7, 42,
		']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', '\\', '.', 56,
		' ', 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
		75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, '>' } } };

struct key_type * parse_scancode(unsigned char c) {

	key->scancode = c;

	key->kind = HIDDEN_KEY;

	switch (c) {
	case CAPSLOCK:
		keyboard.caps_state = !keyboard.caps_state;
		break;
	case LEFT_SHIFT_PRESSED:
	case RIGHT_SHIFT_PRESSED:
		if (keyboard.alt_state == TRUE) {
			keyboard.language = !keyboard.language;
		} else if (keyboard.arrow_right == TRUE) {
			show_next_vt();
		} else if (keyboard.arrow_left == TRUE) {
			show_previous_vt();
		}
	case LEFT_SHIFT_RELEASED:
	case RIGHT_SHIFT_RELEASED:
		keyboard.shift_state = !keyboard.shift_state;
		break;
	case ALT_PRESSED:
		if (keyboard.shift_state == TRUE) {
			keyboard.language = !keyboard.language;
		}
	case ALT_RELEASED:
		keyboard.alt_state = !keyboard.alt_state;
		break;
	case DEAD_KEY:
		if (keyboard.dead_key == TRUE) {
			key->ascii = '«';
			key->kind = ALPHANUM_KEY;
			keyboard.dead_key = FALSE;
		} else if (keyboard.language == SPANISH) {
			keyboard.dead_key = TRUE;
		}
		break;
	case ARROW_RIGHT:
		if (keyboard.shift_state == TRUE) {
			show_next_vt();
		}
		break;
	case ARROW_LEFT:
		if (keyboard.shift_state == TRUE) {
			show_previous_vt();
		}
		break;
	default:
//		si es un key release, ignorar.
		if (c & 0x80) {
			key->ascii = 0;
		} else if (keyboard.dead_key == TRUE && is_vowel(c)) {
// TODO hacer para mayusculas tambien
			switch (c) {
			case 0x12:
				key->ascii = 0x82;
				break;
			case 0x1e:
				key->ascii = 0xa0;
				break;
			case 0x16:
				key->ascii = 0xa3;
				break;
			case 0x17:
				key->ascii = 0xa1;
				break;
			case 0x18:
				key->ascii = 0xa2;
				break;
			}
			key->kind = ALPHANUM_KEY;
			keyboard.dead_key = FALSE;

		} else {

			int state = LOWER;

			if (printable(c)) {
				key->kind = ALPHANUM_KEY;

				if (keyboard.caps_state == TRUE && keyboard.shift_state == FALSE
						&& is_letter(c)) {
					state = UPPER;
				} else if (keyboard.caps_state == FALSE
						&& keyboard.shift_state == TRUE) {
					state = UPPER;
				}

			} else {
				key->kind = FUNCTION_KEY;
			}

			key->ascii = scancode_table[keyboard.language][state][c];
		}
	}
	return key;
}

int printable(unsigned char c) {
	if ((c > 1 && c < 14) || (c > 15 && c < 26) || (c > 29 && c < 42)
			|| (c > 42 && c < 56) || c == 57 || c == 86) {
		return TRUE;
	}
	return FALSE;
}

int is_letter(unsigned char c) {
	if ((c > 15 && c < 26) || (c > 29 && c < 39) || (c > 42 && c < 50))
		return TRUE;
	return FALSE;
}

int is_vowel(unsigned char c) {
	if (c == 0x12 || c == 0x1e || c == 0x16 || c == 0x17 || c == 0x18)
		return TRUE;
	return FALSE;
}

void show_next_vt() {
	if (current_vt == (VT_AMOUNT - 1)) {
		current_vt = 0;
	} else {
		current_vt++;
	}
}

void show_previous_vt() {
	if (current_vt == 0) {
		current_vt = VT_AMOUNT - 1;
	} else {
		current_vt--;
	}
}
