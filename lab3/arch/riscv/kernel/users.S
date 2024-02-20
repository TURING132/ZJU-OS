.section .text.user_program.entry
.align 2

test1: 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 1
    li a1, 1
    addi sp, sp, 8
    j test1

# align with 4K
.align 12

test2: 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 2
    li a1, 2
    addi sp, sp, 8
    j test2

# align with 4K
.align 12

test3: 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 3
    li a1, 3
    addi sp, sp, 8
    j test3

# align with 4K
.align 12

test4: 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    li a1, 4
    addi sp, sp, 8
    j test4

# align with 4K
.align 12

test5: 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 5
    li a1, 5
    addi sp, sp, 8
    j test5

# align with 4K
.align 12