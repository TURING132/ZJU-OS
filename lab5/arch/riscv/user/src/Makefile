USERS_C = $(sort $(wildcard *.c))
USERS_BIN = $(patsubst %.c, %.bin, $(USERS_C))

INCLUDE = -I$(shell pwd)/../lib/include
LIB = $(shell pwd)/../lib/src/*.o

CFLAG   = ${CF} ${INCLUDE}

.PHONY: all clean

all: $(USERS_BIN)

buddy: CFLAG := ${CF} ${INCLUDE} -DBUDDY
buddy: $(USERS_BIN)

%.bin:%.c
	${CC}  ${CFLAG}  -c $< -o $*.o
	${LD} $*.o $(LIB) -o $*
	${OBJCOPY} -O binary $* $@
	rm $*

clean:
	$(shell rm *.bin *.o 2>/dev/null)
