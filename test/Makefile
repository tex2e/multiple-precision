
all: basic.exe arithmetic.exe

basic.exe: MACROS = -D KETA=10
arithmetic.exe: MACROS = -D KETA=200

%.exe: %.c ../integer.c ../integer.h Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o $(basename $<)-integer.o
	gcc -O3 $(MACROS) -I.. $(basename $<)-integer.o $< -o $@
