all: exec

exec: 
	nasm -f elf32 start.s -o start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o task2c.o
	ld -m elf_i386 start.o task2c.o util.o -o ytask2c
	


.PHONY: clean
clean:
	rm -rf ./*.o ytask2c

