.section .text.user_program.entry
.align 2

.incbin "src/test1.bin"

# align with 4K
.align 12

.incbin "src/test2.bin"

# align with 4K
.align 12

.incbin "src/test3.bin"

# align with 4K
.align 12

.incbin "src/test4.bin"

# align with 4K
.align 12

.incbin "src/test5.bin"

# align with 4K
.align 12