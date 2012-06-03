/***************************************************
 Defs.h

 ****************************************************/

#ifndef _defs_
#define _defs_

#define TRUE		1
#define FALSE		0

#define ERROR		-1

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )

#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
	word limit, base_l;
	byte base_m, access, attribs, base_h;
} DESCR_SEG;

/* Descriptor de interrupcion */
typedef struct {
	word offset_l, selector;
	byte cero, access;
	word offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
	word limit;
	dword base;
} IDTR;

/* Tama–o de keyboard->buffer */
#define KEYBOARD_BUFFER_SIZE	10

/* Tama–o de shell->buffer */
#define LINE_BUFFER_SIZE	1024

/* keyboard */
typedef struct keyboard_type {
	int language;
	int state;
	int caps_state;
	int shift_state;
	int alt_state;
	int dead_key;
	char buffer[KEYBOARD_BUFFER_SIZE];
} keyboard_type;

/* Keyboard->language posibles */
#define ENGLISH	0
#define SPANISH	1

#define LANGUAGES 2

/* Keyboard states posibles */
#define LOWER 0
#define UPPER 1

#define STATES 2

/* Entradas en cada tabla de scancodes */
#define KEYS 256

/* key */
typedef struct key_type {
	int kind;
	unsigned char ascii;
	unsigned char scancode;
} key_type;

/* Key->kind posibles */
#define ALPHANUM_KEY	1
#define FUNCTION_KEY	2
#define HIDDEN_KEY	3

/* Scancodes significativos */
#define ALT_PRESSED	0x38
#define ALT_RELEASED	0xb8
#define ARROW_UP		0x48
#define ARROW_LEFT		0x4b
#define ARROW_RIGHT	0x4d
#define ARROW_DOWN		0x50
#define BACKSPACE		0x0d
#define CAPSLOCK		0x3a
#define CONTROL_PRESSED		0x1d
#define CONTROL_RELEASED		0x9d
#define DEAD_KEY		0x1a
#define DELETE			0x53
#define ENTER			0x1c
#define ESCAPE			0x01
#define ESCAPED_KEY	0xe0
#define F1			0x3b
#define F2			0x3c
#define F3			0x3d
#define F4			0x3e
#define LEFT_SHIFT_PRESSED		0x2a
#define LEFT_SHIFT_RELEASED	0xaa
#define NUMLOCK		0x45
#define PAGE_UP		0x49
#define PAGE_DOWN		0x51
#define RIGHT_SHIFT_PRESSED	0x36
#define RIGHT_SHIFT_RELEASED	0xb6
#define SCROLLLOCK		0x46
#define SPACE			0x39
#define TABULAR		0x0f

#define WIDTH 80
#define HEIGHT 25

typedef struct screen_type {
	char video[WIDTH*HEIGHT*2];
	char line_buffer[LINE_BUFFER_SIZE];
	int line_pos;
	int video_pos;
} screen_type;

#endif

