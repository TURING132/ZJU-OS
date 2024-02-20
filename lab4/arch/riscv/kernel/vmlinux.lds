OUTPUT_ARCH( "riscv" )
ENTRY( _start )
MEMORY {
  ram (wxa!ri) : ORIGIN = 0x0000000080000000, LENGTH = 20M
  ramv (wxa!ri) : ORIGIN = 0xffffffc000000000, LENGTH = 4096M
}
PHDRS {
  text PT_LOAD;
  rodata PT_LOAD;
  data PT_LOAD;
  user_program PT_LOAD;
  bss PT_LOAD;
}
SECTIONS {
 . = 0xffffffc000000000;
 .text : 
 {
  PROVIDE(text_start = .);
  *(.text.init)
  *(.text.entry)
  *(.text)
  PROVIDE(text_end = .);
 } >ramv AT>ram :text

 .rodata : ALIGN(0x1000) 
 {
  PROVIDE(rodata_start = .);
  *(.srodata .srodata.*)
  *(.rodata .rodata.*)
  PROVIDE(rodata_end = .);
 } >ramv AT>ram :rodata

 .data : ALIGN(0x1000) 
 {
  PROVIDE(data_start = .);
  *(.sdata .sdata.*)
  *(.data .data.*)
  PROVIDE(data_end = .);
 } >ramv AT>ram :data

 .text.user_program : ALIGN(0x1000)
 {
  PROVIDE(user_program_start = .);
  . = ALIGN(0x1000);
  *(.text.user_program.entry)
 }>ramv AT>ram :user_program

 .bss : ALIGN(0x1000) 
 {
  PROVIDE(bss_start = .);
  *(.sbss .sbss.*)
  *(.bss .bss.*)
  PROVIDE(bss_end = .);
 } >ramv AT>ram :bss


 
 . = ALIGN(0x1000);
 . += 0x1000;
 init_stack_top = .;
 . += 0x1000;
 stack_top = .;

 _end = .;


}
