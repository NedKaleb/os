/* From https://wiki.osdev.org/Writing_A_Page_Frame_Allocator, and modified slightly */
ENTRY (_start)

SECTIONS{
    . = 2M;

    .text BLOCK(4k) : ALIGN(4K)
    {
	text_start = .;
	*(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN (4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN (4K)
    {

        *(.data)
        end_data = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        sbss = .;
        *(COMMON)
        *(.bss)
        ebss = .;
	endkernel = .;
    }
}
