GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _int_0C_hand
GLOBAL  _int_80_hand
GLOBAL  _system_call
GLOBAL  _outb
GLOBAL  _inb
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int_08
EXTERN  int_09
EXTERN  int_0C
EXTERN  int_80


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

_int_0C_hand:				; Handler de COM1

        push    ds
        push    es              ; Se salvan los registros
        pusha                   ; Carga de DS y ES con el valor del selector

		mov eax, 0
		mov dx, 03F8h
        in al, dx
        push ax
        call int_0C

		pop ax
        mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		popa
        pop     es
        pop     ds
        iret

_int_80_hand:				; Handler de INT 80

	push ebp
	mov ebp, esp				;StackFrame

	sti

	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax

	call int_80

	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi

	mov esp, ebp
	pop ebp
	ret

_system_call:

	push ebp
	mov ebp, esp

	push ebx
	push ecx
	push edx
	push esi
	push edi

	mov eax, [ebp + 8] ; Syscall number
	mov ebx, [ebp + 12]; Arg1
	mov ecx, [ebp + 16]; Arg2
	mov edx, [ebp + 20]; Arg3
	mov esi, [ebp + 24]; Arg4
	mov edi, [ebp + 28]; Arg5

	int 80h

	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx

	mov esp, ebp
	pop ebp
	ret

EOI:

	mov	al,20h
	out	20h,al
	popa
	sti
	iret

_inb:

	push ebp
	mov ebp, esp

	push dx
	mov dx, [esp+12]
	mov eax, 0
	in al, dx

;	mov edx, [esp+8]
;	mov ecx, [esp+12]
;	in al, dx
;	mov [ecx], al

	mov esp, ebp
	pop ebp
	ret

_outb:

	push ebp
	mov ebp, esp

	mov eax, [esp+12]
	mov edx, [esp+8]
	out dx, al

	mov esp, ebp
	pop ebp
	ret

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
