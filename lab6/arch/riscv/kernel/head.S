.align 3
.section .text.init
.globl _start
.globl _supervisor
.globl _mtrap
.globl clean_loop
.globl time_interupt
.globl encall_from_s
.globl exit
.globl is_int
.globl other_trap
.extern start_kernel
.extern stack_top
.extern trap_s
.extern bss_start
.extern bss_end
.extern _end
.extern paging_init
.extern init_stack_top

_start:
	# 关闭全局中断使能位 mstatus[mie] = 0
	li t1, 0x8
	csrc mstatus, t1

	# 关闭 M 模式各类型中断使能位
	# mie[meie, mtie, msie] = 0
	li t1, 0x888
	csrc mie, t1

	# 设置 M 模式触发异常跳转到 _mtrap 去处理
	la t1, _mtrap
	csrw mtvec, t1

	# 用 mscratch 存储 M 模式下的栈指针
	la t1, stack_top
	csrw mscratch, t1

	# 时钟中断委托给 S 模式处理
	li t1, 0x20
	csrs mideleg, t1

	# 将 page fault 异常全部委托给 S 模式处理
	li t1, 0xB000
	csrs medeleg, t1

	# DONE: 把用户系统调用（syscall_from_U_mode）委托给 S 模式处理
	li t1, 0x100
	csrs medeleg, t1


	# .bss 段全部置 0
	la t1, bss_start
	la t2, bss_end
clean_loop:
	sb zero, 0(t1)           # 将 0 填充到 t1 这个地址
	addi t1, t1, 1           # t1 ++
	bne t1, t2, clean_loop   # 如果 t1 != t2，跳转到 clean_loop 继续循环

	# 打开中断使能，并设置spp、mpp使得mret时回到S态
	# mstatus[mpp, spp, spie, mpie] = 1
	li t1, 0x9a0
	csrs mstatus, t1
	li t1, 0x1000
	csrc mstatus, t1

	# 打开时钟中断使能
	# mie[mtie, stie] = 1
	li t1, 0x0a0
	csrs mie, t1

	# 准备跳转地址, mret 将会跳转到 mepc 位置执行
	la t1, _supervisor
	csrw mepc, t1
	mret            

_supervisor:
	# DONE: 
	# 1. 在 _supervisor 开头先设置 satp 寄存器为0，暂时关闭 MMU
	# 2. 设置 sp 的值为 init_stack_top 的物理地址
	# 3. 调用 paging_init 函数建立页表
	# 4. 设置 satp 的值以打开 MMU
	# 提示：从 _end 开始为页表分配物理空间，即根页表的地址为 _end
	# 5. 执行 sfence.vma 指令同步虚拟内存相关映射
	# 6. 设置 stvec 为异常处理函数 trap_s 在虚拟地址空间下的地址
	# 提示：vmlinux.lds 中规定将内核放在物理内存 0x80000000、虚拟内存 0xffffffc000000000 的位置，因此物理地址空间下的地址 x 在虚拟地址空间下的地址为 x - 0x80000000 + 0xffffffe00000000。
	# 7. 记录 start_kernel 在虚拟地址空间下的地址，加载到 s0 寄存器中
	# 8. 设置 sp 的值为虚拟地址空间下的 init_stack_top
	# 9. 设置sstatus.spp = 0
	# 10. 设置sstatus.sum = 1，使得S态可以访问U态的地址空间，便于后续的字符串输出
	# 11. 使用 jr 指令跳转到 start_kernel（读取之前记录在寄存器中的值）

	# 关闭 MMU
	li t1, 0
	csrw satp, t1

	# 设置 sp 的值为 init_stack_top 的物理地址
	la sp, init_stack_top

	# 建立页表
	call paging_init

	# 打开 MMU，根页表的物理地址为_end
	la t1, _end
	srli t1, t1, 12
	csrw satp, t1
	li t1, 0x8000000000000000
	csrs satp, t1

	# 同步虚拟内存相关映射
	sfence.vma

	# 设置 stvec 为异常处理函数 trap_s 在虚拟地址空间下的地址
	li t1, 0xffffffc000000000
	li t2, 0x80000000
	la t3, trap_s
	add t3, t3, t1
	sub t3, t3, t2
	csrw stvec, t3

	# 记录 start_kernel 在虚拟地址空间下的地址，加载到 s0 寄存器中
	la s0, start_kernel
	add s0, s0, t1
	sub s0, s0, t2

	# 设置 sp 的值为虚拟地址空间下的 init_stack_top
	la sp, init_stack_top
	add sp, sp, t1
	sub sp, sp, t2

	# sstatus[spp] = 0，使得sret时回到U态
	li t1, 0x100
	csrc sstatus, t1

	# 设置sstatus.sum = 1，使得S态可以访问U态的地址空间，便于后续的字符串输出
	li t1, 0x40000
	csrs sstatus, t1

	# 跳转到 start_kernel
	jr s0




_mtrap:
	# 交换 mscratch 和 sp
	# 相当于使用 M 模式的栈指针
	csrrw sp, mscratch, sp

	# 保存寄存器
	addi sp, sp, -264
	sd sp, 0(sp)
	sd ra, 8(sp)
	sd gp, 16(sp)
	sd tp, 24(sp)
	sd t0, 32(sp)
	sd t1, 40(sp)
	sd t2, 48(sp)
	sd s0, 56(sp)
	sd s1, 64(sp)
	sd a0, 72(sp)
	sd a1, 80(sp)
	sd a2, 88(sp)
	sd a3, 96(sp)
	sd a4, 104(sp)
	sd a5, 112(sp)
	sd a6, 120(sp)
	sd a7, 128(sp)
	sd s2, 136(sp)
	sd s3, 144(sp)
	sd s4, 152(sp)
	sd s5, 160(sp)
	sd s6, 168(sp)
	sd s7, 176(sp)
	sd s8, 184(sp)
	sd s9, 192(sp)
	sd s10, 200(sp)
	sd s11, 208(sp)
	sd t3, 216(sp)
	sd t4, 224(sp)
	sd t5, 232(sp)
	sd t6, 240(sp)
	csrr t0, mstatus
	csrr t1, mepc
	sd t0, 248(sp)
	sd t1, 256(sp)

	# 检查 mcause，用最高位判断是中断还是异常
	csrr t0, mcause
	srai t2, t0, 63
	bnez t2, is_int

	# 判断是不是 S 模式主动发出的异常
	li t1, 9
	beq t0, t1, encall_from_s
	j other_trap

	# 判断是不是 machine timer interrupt
	# 是 --> 跳转到 time_interupt
	# 否 --> 跳转到 other_trap
is_int:
	andi t0, t0, 0x7ff
	li t1, 7
	beq	t0, t1, time_interupt
	j other_trap

time_interupt:
	# 禁用时钟中断
	li t1, 0x80
	csrc mie, t1

	# 设置 stip 为 1，这样才能进入 S 模式的时钟中断处理
	li t1, 0x20
	csrs mip, t1

	# 恢复寄存器 mepc 和 mstatus
	ld t0, 248(sp)
	ld t1, 256(sp)
  	csrw mstatus, t0
  	csrw mepc, t1
	j exit

encall_from_s:
	# 设置下一次时钟中断
	li t1, 0x2004000         # t1 = mtimecmp 的地址
	ld t0, 0(t1)             # t0 = *t1
	li t3, 1000000           # t3 = 1000000
	add t0, t0, t3           # t0 = t0 + t3
	sd t0, 0(t1)             # *mtimecmp = t0

	# 清除 stip
	li t1, 0x20
	csrc mip, t1

	# 开启时钟中断
	li t1, 0x80
	csrs mie, t1

	# 恢复寄存器 mepc 和 mstatus
	ld t0, 248(sp)
	ld t1, 256(sp)
	addi t1, t1, 4  # 主动发出的异常，mepc 需要移动到下一条指令
	csrw mstatus, t0
	csrw mepc, t1
	j exit


# 我们对其他异常不做任何处理
other_trap:
	# 恢复寄存器 mepc 和 mstatus
	ld t0, 248(sp)
	ld t1, 256(sp)
  	csrw mstatus, t0
  	csrw mepc, t1

exit:
	#恢复寄存器
	ld sp, 0(sp)
	ld ra, 8(sp)
	ld gp, 16(sp)
	ld tp, 24(sp)
	ld t0, 32(sp)
	ld t1, 40(sp)
	ld t2, 48(sp)
	ld s0, 56(sp)
	ld s1, 64(sp)
	ld a0, 72(sp)
	ld a1, 80(sp)
	ld a2, 88(sp)
	ld a3, 96(sp)
	ld a4, 104(sp)
	ld a5, 112(sp)
	ld a6, 120(sp)
	ld a7, 128(sp)
	ld s2, 136(sp)
	ld s3, 144(sp)
	ld s4, 152(sp)
	ld s5, 160(sp)
	ld s6, 168(sp)
	ld s7, 176(sp)
	ld s8, 184(sp)
	ld s9, 192(sp)
	ld s10, 200(sp)
	ld s11, 208(sp)
	ld t3, 216(sp)
	ld t4, 224(sp)
	ld t5, 232(sp)
	ld t6, 240(sp)
	addi sp, sp, 264

	# 交换 mscratch 和 sp
	# 相当于恢复到异常前的 sp
	csrrw sp,mscratch,sp
	mret