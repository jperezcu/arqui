GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08
EXTERN  int_09


SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:					; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int_08_hand:				; Handler de INT 8 (Timer tick)

    pusha
	call    int_08
	jmp	EOI					; Envio de EOI generico al PIC

_int_09_hand:				; Handler de INT 9 (Keyboard)

	cli
	pusha

	in 	al, 60h
	push 	eax
	call	int_09
	pop	eax

	jmp	EOI					; Envio de EOI generico al PIC

EOI:

	mov	al,20h
	out	20h,al
	popa
	sti
	iret

; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0
;


_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
