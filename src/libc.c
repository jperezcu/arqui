#include "../include/kc.h"
#include <stdarg.h>

extern vt_type * vt;
extern int current_vt;

/***************************************************************
 *k_clear_screen
 *
 * Borra la pantalla en modo texto color.
 ****************************************************************/

void k_clear_screen() {
	char *vidmem = (char *) 0xb8000;
	unsigned int i = 0;
	while (i < (80 * 25 * 2)) {
		vidmem[i] = ' ';
		i++;
		vidmem[i] = WHITE_TXT;
		i++;
	};
}

/***************************************************************
 *setup_IDT_entry
 * Inicializa un descriptor de la IDT
 *
 *Recibe: Puntero a elemento de la IDT
 *	 Selector a cargar en el descriptor de interrupcion
 *	 Puntero a rutina de atencion de interrupcion
 *	 Derechos de acceso del segmento
 *	 Cero
 ****************************************************************/

void setup_IDT_entry(DESCR_INT *item, byte selector, dword offset, byte access,
		byte cero) {
	item->selector = selector;
	item->offset_l = offset & 0xFFFF;
	item->offset_h = offset >> 16;
	item->access = access;
	item->cero = cero;
}

void putc(unsigned char c) {
	char c_copy = c;
	_system_call(WRITE, STDOUT, &c_copy, 1);
}

void putd(long int d) {
	int i=31;
	char aux[32];
	while(i>=0 && d >0 ){
		aux[i]=(d%10)+'0';
		d/=10;
		i--;		
		}
		i++;
	for(;i<32;i++){
		putc(aux[i]);
	}

	
}

unsigned char getc() {
	char c = -1;
	_system_call(READ, STDIN, &c, 1);
	return c;
}

void write(int device, char * buffer, int amount) {
	if (device == STDOUT) {
		int i;
		for (i = 0; i < amount; i++) {
			print(buffer[i]);
		}
	}
}

void read(int device, char * buffer, int amount) { // siempre vamos a leer con amount=1...
	if (device == STDIN) {
		int i;
		for (i = 0; i < amount && keyboard_buffer_can_read(); i++) {
			buffer[i] = get_char_from_keyboard_buffer();
		}
	}
}

void printf(char * string, ...) {
	int i = 0;
	int j;
	int int_value;
	char char_value;
	char * string_value;
	char next_char;

	va_list args; // apunta a la lista de argumentos

	va_start(args, string); // inicializa args para que apunte al primer argumento variable

	while (string[i] != '\0') {
		if (string[i] != '%' && string[i] != '\\') {
			putc(string[i]); //letra comun
		} else {
			switch (string[i]) {
			case '%':
			{
				next_char = string[i + 1];
				switch (next_char) {
					case 'd': //llega %d
						int_value = va_arg(args,int);
						putd(int_value);
						i++;
						break;
					case 'c':
					case 's': //llega %d
						string_value = va_arg(args,char*);
						for(j=0; string_value[j] != '\0';j++){
							putc(string_value[j]);
						}
						i++;
						break;
					default:
						putc(string[i]); //llega % seguido de otra cosa
						break;
					}
				}
					break;
				case '\\':
					next_char = string[i + 1];
					if (next_char == 'n') {
						putc('\n'); //llega /n
					}	 else {
						putc('\\'); //llega / seguido de otra cosa
					}
					break;
				}

			}
			i++;

		}
	va_end(args);

	}
