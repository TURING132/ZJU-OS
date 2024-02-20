BASE_ADDR = 0x10000000;
SECTIONS
{
	. = BASE_ADDR;
	.text : {
		*(.text.init)
		*(.text.*)
	 }
	.rodata : { 
		*(.rodata) 
		*(.rodata.*) 
	}
	.data : { 
		*(.data) 
		*(.data.*)
	}
	.bss : { 
		*(.sbss)
		*(.sbss.*)
		*(.bss)
		*(.bss.*)
	}
}