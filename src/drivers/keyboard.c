#include "../../include/defs.h"
#include "../../include/keyboard.h"

struct key_type * key;
extern keyboard_type keyboard;

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
	default:
		if (c & 0x80) {
			key->ascii = 0;
		} else {
			if (printable(c)) {
				key->kind = ALPHANUM_KEY;

				if (keyboard.caps_state != keyboard.shift_state) {
					keyboard.state = UPPER;
				} else {
					keyboard.state = LOWER;
				}
			}

			key->ascii = scancode_table[keyboard.language][keyboard.state][c];
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
