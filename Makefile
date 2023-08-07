CFLAGS := -Wall -Wextra -g
SDL2_FLAGS := -lSDL2 -I/usr/include/SDL2/
objects := src/chip8.o

.PHONY: all
all: main.out

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

main.out: src/main.c src/chip8.o
	cc $(CFLAGS) src/main.c src/chip8.o -o main.out $(SDL2_FLAGS)

.PHONY: clean
clean:
	rm *.out src/*.o