
all: basic.exe arithmetic.exe

basic.exe: MACROS = -D KETA=10
basic.exe: ../integer.c ../integer.h basic.c Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o basic-integer.o
	gcc -O3 $(MACROS) -I.. basic-integer.o basic.c -o basic.exe

arithmetic.exe: MACROS = -D KETA=200
arithmetic.exe: ../integer.c ../integer.h arithmetic.c Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o arithmetic-integer.o
	gcc -O3 $(MACROS) -I.. arithmetic-integer.o arithmetic.c -o arithmetic.exe
