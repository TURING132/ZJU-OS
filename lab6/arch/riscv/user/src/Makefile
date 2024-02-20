USERS_C = $(sort $(wildcard *.c))
USERS_BIN = $(patsubst %.c, %.bin, $(USERS_C))

INCLUDE = -I$(shell pwd)/../lib/include
LIB = $(shell pwd)/../lib/src/*.o

CFLAG   = ${CF} ${INCLUDE}

.PHONY: all clean

all: $(USERS_BIN)

head.o: head.s
	${CC}  ${CFLAG}  -c $<

%.bin: %.c head.o $(LIB)
	${CC}  ${CFLAG} -O0 -c $< -o $*.o
	${LD} -T user.lds $*.o head.o $(LIB) -o $*
	${OBJCOPY} -O binary $* $@
	rm $*

clean:
	$(shell rm *.bin *.o 2>/dev/null)
