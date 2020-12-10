jc : jc.c assembly.c assembly.h token.c token.h stack.c stack.h 
	clang -Wall -g -o jc jc.c assembly.c token.c stack.c
clean : jc
	rm -f jc *.o