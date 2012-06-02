#include "../../include/defs.h"
#include "../../include/keyboard.h"

struct key_type * key;
keyboard_type * keyboard;

unsigned char scancode_table[LANGUAGES][STATES][KEYS] = {
	{
		{ 0, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\\', 0,
		'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '\\', '.', 56,
		' ', 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
		75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, '<' },

		{ 0, 0x1b, '!', '@',
		'#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W',
		'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0xb3, 0, '|', 'Z', 'X',
		'C', 'V', 'B', 'N', 'M', '<', '>', '?', '\\', '.', 56, ' ', 58, 59, 60,
		61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
		79, 80, 81, 82, 83, 84, 85, '>' }
	},
	{
		{ 0, 0xb3, '1', '2', '3', '4', '5',
		'6', '7', '8', '9', '0', '\'', 0xa8, '\b', '\t', 'q', 'w', 'e', 'r',
		't', 'y', 'u', 'i', 'o', 'p', '´', '+', '\n', 0, 'a', 's', 'd', 'f',
		'g', 'h', 'j', 'k', 'l', 0xa4, '{', 0xb3, 0, '}', 'z', 'x', 'c', 'v',
		'b', 'n', 'm', ',', '.', '-', '\\', '.', 56, ' ', 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
		81, 82, 83, 84, 85, '<' },

		{ 0, 0xa7, '!', '"', '#', '$', '%', '&', '/',
		'(', ')', '=', '?', 0xad, '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
		'I', 'O', 'P', '¨', '*', '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J',
		'K', 'L', 0xa5, '[', 0xa7, 42, ']', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		';', ':', '_', '\\', '.', 56, ' ', 58, 59, 60, 61, 62, 63, 64, 65, 66,
		67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
		85, '>' }
	}
};

struct key_type * parse_scancode(unsigned char c) {

	key->kind = HIDDEN_KEY;
	switch (c) {
		case NUMLOCK:
			keyboard->num_state =
					!keyboard->num_state;
			break;
		case SCROLLLOCK:
			keyboard->scroll_state =
					!keyboard->scroll_state;
			break;
		case LEFT_SHIFT_PRESSED:
		case RIGHT_SHIFT_PRESSED:
			keyboard->shift_state = TRUE;
//			if (keyboard->alt_state == TRUE)
//				change_language();
			break;
		case RIGHT_SHIFT_RELEASED:
		case LEFT_SHIFT_RELEASED:
			keyboard->shift_state = FALSE;
			break;
		case CAPSLOCK:
			keyboard->caps_state =
					!keyboard->caps_state;
			break;
		case ESCAPED_KEY:
			keyboard->escaped_key = TRUE;
			break;
		case ALT_PRESSED:
			keyboard->alt_state = TRUE;
//			if (keyboard->shift_state == TRUE)
//				change_language();
			break;
		case ALT_RELEASED:
			keyboard->alt_state = FALSE;
			break;
		case CONTROL_PRESSED:
			keyboard->ctrl_state = TRUE;
			break;
		case CONTROL_RELEASED:
			keyboard->ctrl_state = FALSE;
			break;
		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
		case PAGE_UP:
		case PAGE_DOWN:
			key->kind = FUNCTION_KEY;
			break;
		case F1:
		case F2:
		case F3:
		case F4:
			key->kind = FUNCTION_KEY;
			break;
		case DEAD_KEY:
			if (keyboard->language == SPANISH) {
				keyboard->dead_key = TRUE;
				break;
			}
	}



	if (c & 0x80) {
		key->kind = HIDDEN_KEY;
	} else {
		if (printable(c)) {
			key->kind = ALPHANUM_KEY;
			key->ascii = scancode_table[SPANISH][LOWER][c];
		}
	}
	return key;
}

void change_language(){

}

int printable(unsigned char c) {
	if ((c > 1 && c < 29) || (c > 29 && c < 42) || (c > 42 && c < 55) || c == 86
			|| c == 57) {
		return TRUE;
	}
	return FALSE;
}
