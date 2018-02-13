
arithmetic.exe: ../integer.c ../integer.h arithmetic.c
	gcc -O3 -D KETA=100 -c ../integer.c -o arithmetic-integer.o
	gcc -O3 -D KETA=100 -I.. arithmetic-integer.o arithmetic.c -o arithmetic.exe
