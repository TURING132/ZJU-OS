# TODO: 将本文件夹下的所有.S与.c文件编译成.o文件	
# 1. 使用wildcard获取所有.S与.c文件（TODO）
ASM_SRC= $(sort $(wildcard *.S))
C_SRC= $(sort $(wildcard *.c))

# 2. 使用patsubst将.S与.c文件转换成.o文件（TODO）
ASM_OBJ= $(patsubst %.S,%.o,$(ASM_SRC))
C_OBJ= $(patsubst %.c,%.o,$(C_SRC))

# 3. 定义目标文件all与依赖关系。执行make命令时可以指定编译目标，比如make all，make clean，然后make会寻找该编译目标，并根据make的运行机制进行编译。（TODO）
all: $(ASM_OBJ) $(C_OBJ)

# 4. 使用%.o:%.S与%.o:%.c定义依赖关系与命令（Done）
%.o:%.S
	${CC}  ${CFLAG}  -c $<
%.o:%.c
	${CC}  ${CFLAG}  -c $<

# 请自行查询makefile相关文档，包括makefile函数、变量、通配符，make的运行机制等



clean:
	$(shell rm *.o 2>/dev/null)
