
all: basic.exe arithmetic.exe

basic.exe: ../integer.c ../integer.h basic.c Makefile
	gcc -O3 -D KETA=10 -c ../integer.c -o basic-integer.o
	gcc -O3 -D KETA=10 -I.. basic-integer.o basic.c -o basic.exe -lm

arithmetic.exe: ../integer.c ../integer.h arithmetic.c Makefile
	gcc -O3 -D KETA=200 -D RADIX=1000 -D RADIX_LEN=3 -c ../integer.c -o arithmetic-integer.o
	gcc -O3 -D KETA=200 -D RADIX=1000 -D RADIX_LEN=3 -I.. arithmetic-integer.o arithmetic.c -o arithmetic.exe -lm
