
C_FILES = $(wildcard *.c)
EXE_FILES = $(patsubst %.c,%.exe,$(C_FILES))

all: $(EXE_FILES)

basic.exe:      MACROS = -D KETA=10
arithmetic.exe: MACROS = -D KETA='(200/RADIX_LEN+10)'
extra.exe:      MACROS = -D KETA='(200/RADIX_LEN+10)'

%.exe: %.c ../integer.c ../integer.h Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o $(basename $<)-integer.o
	gcc -O3 $(MACROS) -I.. $(basename $<)-integer.o $< -o $@
