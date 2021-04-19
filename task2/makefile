all: exec

exec: 
	nasm -f elf32 start.s -o start.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2b.c -o task2b.o
	ld -m elf_i386 start.o task2b.o util.o -o task2
	


.PHONY: clean
clean:
	rm -rf ./*.o task2

