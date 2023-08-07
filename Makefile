CFLAGS := -Wall -Wextra -g
objects := src/chip8.o

.PHONY: all
all: main.out

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

main.out: src/main.c src/chip8.o
	cc $(CFLAGS) src/main.c src/chip8.o -o main.out

.PHONY: clean
clean:
	rm *.out src/*.o