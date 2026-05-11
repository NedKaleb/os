memMapEntries equ 0x8500	; Where the number of memory map entries will be stored

;; In the end, 0x8500 will have the number of available memory map entries, and 0x8504 will be the first of those entries
;; Carry flag will be set on error
global getMemoryMap	
getMemoryMap:
	mov di, 0x8504			; Base address of the memory map entries table
	xor ebx, ebx
	xor bp, bp
	mov edx, 0x0534D4150		; 'SMAP'
	mov eax, 0xE820			; BIOS function
	mov [es:di + 20], dword 1	; Forces a valid entry for ACPI 3.x
	mov ecx, 24			; Maximum size in bytes for each entry

	int 0x15			; The interrupt being used
	jc .error			; Carry bit will be set by the interrupt to indicate and error

	mov edx, 0x0534D4150		; 'SMAP'
	cmp eax, edx			; On a successful call, eax = 'SMAP'
	jne .error			; Something's gone wrong on the BIOS's end (I think)
	test ebx, ebx			; If ebx == 0, there is only 1 or zero available entries
	je .error
	jmp .start			; Everything is successful and a valid entry has been found

.nextEntry:
	mov ecx, 24			; Resets ecx in case it has been changed
	mov eax, 0xE820			; Some with eax
	mov [es:di + 20], dword 1	; Force a valid ACPI 3.X entry
	int 0x15
	jc .done			; Reached the end of the list
	mov edx, 0x0534D4150		; Repair potentially trashed register
	
.start:
	jcxz .skipEntry		; The given memory map entry is zero bytes in length, and so should be skipped
	cmp cl, 20		; Tests if it got a 24-byte response
	jbe .noText
	test byte [es:di + 20], 1 ; Tests if the "ignore this data" bit  is clear
	je .skipEntry
	
.noText:	
	mov ecx, [ES:DI+8]	; ecx = lower 32bits of length
	or ecx, [ES:DI+12]	; Combines with higher 32bits of length
	jz .skipEntry		; If uint64_t length == 0, skip entry
	
.goodEntry:
	inc bp			; Number of entries++
	add di, 24		; Each entry is 24 bytes in length

.skipEntry:
	test ebx, ebx		; If ebx == 0, we're done
	jz .done
	jmp .nextEntry

;; TODO: Add error handling in getMemoryMap 
.error:
	stc			; Sets carry flag
	ret

.done:
	mov [memMapEntries], bp	; memMapEntries = number of entries
	clc			; Clears carry flag
	ret
