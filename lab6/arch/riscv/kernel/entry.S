.section .text.entry

.extern test
.global trap_s
.global trap_s_bottom
.extern handler_s
.equ reg_size, 0x8
.align 2

trap_s:
	# DONE: swap sp and sscratch, now sp point to kernel stack, sscratch point to user stack
	csrrw sp, sscratch, sp

	# save the caller saved registers and sepc
	addi sp, sp, -reg_size*31

	sd ra, 0*reg_size(sp)
	sd t0, 1*reg_size(sp)
	sd t1, 2*reg_size(sp)
	sd t2, 3*reg_size(sp)
	sd a0, 4*reg_size(sp)
	sd a1, 5*reg_size(sp)
	sd a2, 6*reg_size(sp)
	sd a3, 7*reg_size(sp)
	sd a4, 8*reg_size(sp)
	sd a5, 9*reg_size(sp)
	sd a6, 10*reg_size(sp)
	sd a7, 11*reg_size(sp)
	sd t3, 12*reg_size(sp)
	sd t4, 13*reg_size(sp)
	sd t5, 14*reg_size(sp)
	sd t6, 15*reg_size(sp)
	csrr a0, sepc
	sd a0, 16*reg_size(sp)
	sd s0, 17*reg_size(sp)
	sd s1, 18*reg_size(sp)
	sd s2, 19*reg_size(sp)
	sd s3, 20*reg_size(sp)
	sd s4, 21*reg_size(sp)
	sd s5, 22*reg_size(sp)
	sd s6, 23*reg_size(sp)
	sd s7, 24*reg_size(sp)
	sd s8, 25*reg_size(sp)
	sd s9, 26*reg_size(sp)
	sd s10, 27*reg_size(sp)
	sd s11, 28*reg_size(sp)
	sd gp, 29*reg_size(sp)
	sd tp, 30*reg_size(sp)

	# call handler_s(scause)
	csrr a0, scause
	csrr a1, sepc
	mv a2, sp
	jal handler_s

trap_s_bottom:
	# load sepc and caller saved registers
	ld ra, 0*reg_size(sp)
	ld t0, 1*reg_size(sp)
	ld t1, 2*reg_size(sp)
	ld t2, 3*reg_size(sp)
	ld a0, 4*reg_size(sp)
	ld a1, 5*reg_size(sp)
	ld a2, 6*reg_size(sp)
	ld a3, 7*reg_size(sp)
	ld a4, 8*reg_size(sp)
	ld a5, 9*reg_size(sp)
	ld a6, 10*reg_size(sp)
	ld a7, 11*reg_size(sp)
	ld t3, 12*reg_size(sp)
	ld t4, 13*reg_size(sp)
	ld t5, 14*reg_size(sp)
	ld t6, 16*reg_size(sp)
	csrw sepc, t6
	ld t6, 15*reg_size(sp)
	ld s0, 17*reg_size(sp)
	ld s1, 18*reg_size(sp)
	ld s2, 19*reg_size(sp)
	ld s3, 20*reg_size(sp)
	ld s4, 21*reg_size(sp)
	ld s5, 22*reg_size(sp)
	ld s6, 23*reg_size(sp)
	ld s7, 24*reg_size(sp)
	ld s8, 25*reg_size(sp)
	ld s9, 26*reg_size(sp)
	ld s10, 27*reg_size(sp)
	ld s11, 28*reg_size(sp)
	ld gp, 29*reg_size(sp)
	ld tp, 30*reg_size(sp)

	addi sp, sp, reg_size*31

	# DONE: swap sp and sscratch, now sp point to user stack, sscratch point to kernel stack
	csrrw sp, sscratch, sp

	sret


.globl __switch_to
__switch_to:
	li    a4,  40 
	add   a3, a0, a4
	add   a4, a1, a4
	# DONE: Save context into prev->thread

	sd  ra, 0*reg_size(a3)
	sd  sp, 1*reg_size(a3)
	sd  s0, 2*reg_size(a3)
	sd  s1, 3*reg_size(a3)
	sd  s2, 4*reg_size(a3)
	sd  s3, 5*reg_size(a3)
	sd  s4, 6*reg_size(a3)
	sd  s5, 7*reg_size(a3)
	sd  s6, 8*reg_size(a3)
	sd  s7, 9*reg_size(a3)
	sd  s8, 10*reg_size(a3)
	sd  s9, 11*reg_size(a3)
	sd s10, 12*reg_size(a3)
	sd s11, 13*reg_size(a3)
	
	# DONE: save sscratch into prev->sscratch
	csrr s0, sscratch
	sd s0, 14*reg_size(a3)
	# DONE: save satp into prev->satp
	csrr s0, satp
	sd s0, 15*reg_size(a3)
  
	# DONE: Restore context from next->thread
	ld  ra, 0*reg_size(a4)
	ld  sp, 1*reg_size(a4)
	ld  s1, 3*reg_size(a4)
	ld  s2, 4*reg_size(a4)
	ld  s3, 5*reg_size(a4)
	ld  s4, 6*reg_size(a4)
	ld  s5, 7*reg_size(a4)
	ld  s6, 8*reg_size(a4)
	ld  s7, 9*reg_size(a4)
	ld  s8, 10*reg_size(a4)
	ld  s9, 11*reg_size(a4)
	ld s10, 12*reg_size(a4)
	ld s11, 13*reg_size(a4)

	# DONE: load sscratch from next->sscratch
	ld s0, 14*reg_size(a4)
	csrw sscratch, s0

	# DONE: load satp from next->satp
	ld s0, 15*reg_size(a4)
	csrw satp, s0

	ld  s0, 2*reg_size(a4)
  
	# return to ra
	ret

.globl __init_sepc
__init_sepc:
	li t0, 0x1000000
    csrw sepc, t0
    csrrw sp, sscratch, sp
    sret
