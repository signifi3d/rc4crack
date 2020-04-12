all: main.o brute.o pos_stack.o decrypt.o dict.o
	gcc -O2 main.o brute.o pos_stack.o decrypt.o dict.o -o rc4crack

main.o: main.c
	gcc -O2 -c main.c

brute.o: brute.c
	gcc -O2 -c brute.c

pos_stack.o: pos_stack.c
	gcc -O2 -c pos_stack.c

decrypt.o: decrypt.c
	gcc -O2 -c decrypt.c

dict.o: dict.c
	gcc -O2 -c dict.c

clean:
	rm *.o rc4crack
