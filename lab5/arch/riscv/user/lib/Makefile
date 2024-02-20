INCLUDE = -I$(shell pwd)/include

CFLAG   = ${CF} ${INCLUDE} 

C_SRC = $(sort $(wildcard src/*.c))

OBJ = $(patsubst %.c,%.o,$(C_SRC))

.PHONY: all clean

all: $(OBJ)

%.o:%.c
	${CC}  ${CFLAG}  -c $< -o $@

clean:
	$(shell rm src/*.o 2>/dev/null)
