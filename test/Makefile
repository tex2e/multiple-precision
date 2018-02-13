
arithmetic.exe: ../integer.c ../integer.h arithmetic.c Makefile
	gcc -O3 -D KETA=200 -c ../integer.c -o arithmetic-integer.o
	gcc -O3 -D KETA=200 -I.. arithmetic-integer.o arithmetic.c -o arithmetic.exe -lm
