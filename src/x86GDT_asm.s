global gdtFlush
extern gp	
gdtFlush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	jmp 0x08:flush2		; Jumps to protected mode and writes to the cs register
	
bits 32
flush2:	
	ret
