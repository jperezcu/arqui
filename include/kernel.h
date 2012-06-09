#include "../include/defs.h"

/********************************** 
*
*  Kernel
*
***********************************/

#ifndef _kernel_
#define _kernel_

/* Definir‡ las entradas del IDT */
void setup_IDT_content();

/* Definir‡ el IDTR */
void setup_IDTR();

/* Definir‡ las terminales virtuales */
void setup_vts();

void setup_keyboard_buffer();

void setup_serial_port();

void change_terminal();


#endif
