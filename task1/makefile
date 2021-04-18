all: exec

exec: 
	nasm -f elf32 start.s -o start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector encoder.c -o encoder.o
	ld -m elf_i386 start.o encoder.o util.o -o task1
	


.PHONY: clean
clean:
	rm -rf ./*.o task1

