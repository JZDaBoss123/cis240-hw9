jc : jc.c assembly.c assembly.h token.c token.h
	clang -Wall -g -o jc jc.c assembly.c token.c
clean : trace
	rm -f trace *.o