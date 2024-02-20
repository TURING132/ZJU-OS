.align 3
.section .text.init
.globl _start
.extern main
.extern exit

_start:
    call main
    call exit
