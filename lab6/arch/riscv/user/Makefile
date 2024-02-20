all: users.o

.PHONY: all clean users.o

users.o:users.S
	make -C lib
	make -C src
	${CC}  ${CFLAG}  -c $<

clean:
	make -C lib clean
	make -C src clean
	$(shell rm *.o 2>/dev/null)
