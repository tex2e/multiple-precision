
n0.exe: MACROS = -D KETA='(1200/RADIX_LEN)'
n1.exe: MACROS = -D KETA='(1200/RADIX_LEN)'
n2.exe: MACROS = -D KETA='(1200/RADIX_LEN)'
n3.exe: MACROS = -D KETA='(1200/RADIX_LEN)'
n4.exe: MACROS = -D KETA='(3570/RADIX_LEN+10)'
n5.exe: MACROS = -D KETA='(2010/RADIX_LEN+10)'
n6.exe: MACROS = -D KETA='(2000/RADIX_LEN+10)'
n7.exe: MACROS = -D KETA='(1010/RADIX_LEN+10)'
n8.exe: MACROS = -D KETA='(30/RADIX_LEN)'
n9.exe: MACROS = -D KETA='(1100/RADIX_LEN+10)'
n10.exe: MACROS = -D KETA='(30/RADIX_LEN)'

%.exe: %.c ../integer.c ../integer.h Makefile
	gcc -O3 $(MACROS) -c ../integer.c -o $(basename $<)-integer.o
	gcc -O3 $(MACROS) -I.. $(basename $<)-integer.o $< -o $@
