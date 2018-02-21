
C_FILES = $(wildcard *.c)
EXE_FILES = $(patsubst %.c,%.exe,$(C_FILES))

all: $(EXE_FILES)

n00.exe: MACROS = -D KETA='(1100/RADIX_LEN+10)'
n01.exe: MACROS = -D KETA='(1100/RADIX_LEN+10)'
n02.exe: MACROS = -D KETA='(1100/RADIX_LEN+10)'
n03.exe: MACROS = -D KETA='(1100/RADIX_LEN+10)'
n04.exe: MACROS = -D KETA='(3570/RADIX_LEN+10)'
n05.exe: MACROS = -D KETA='(2000/RADIX_LEN+10)'
n06.exe: MACROS = -D KETA='(1000/RADIX_LEN+10)'
n07.exe: MACROS = -D KETA='(1000/RADIX_LEN+10)'
n08.exe: MACROS = -D KETA='(30/RADIX_LEN)'
n09.exe: MACROS = -D KETA='(1000/RADIX_LEN+10)'
n10.exe: MACROS = -D KETA='(30/RADIX_LEN)'

%.exe: %.c ../integer.c ../integer.h Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o $(basename $<)-integer.o
	gcc -O3 $(MACROS) -I.. $(basename $<)-integer.o $< -o $@

exec:
	@for file in $(patsubst %.c,%.exe,$(C_FILES)); do \
		echo "===== $$file ====="; \
		time ./$$file; \
		echo; \
	done

clean:
	$(RM) *.exe *.o
