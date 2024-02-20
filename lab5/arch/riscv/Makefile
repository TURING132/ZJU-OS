all:
	${MAKE} -C kernel
	${MAKE} -C user

buddy:
	${MAKE} -C kernel
	${MAKE} -C user buddy

clean:
	${MAKE} -C kernel clean
	${MAKE} -C user clean
	$(shell test -d boot && rm -rf boot)
