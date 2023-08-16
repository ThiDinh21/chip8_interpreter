CFLAGS := -Wall -Wextra -g
SDL2_FLAGS := -lSDL2 -I/usr/include/SDL2/
OBJECTS := src/chip8.o src/graphics.o

.PHONY: all
all: main.out

%.o: %.c
	gcc -c $(CFLAGS) $< -o $@

main.out: src/main.c $(OBJECTS)
	cc $(CFLAGS) src/main.c $(OBJECTS) -o main.out $(SDL2_FLAGS) -lm

.PHONY: clean
clean:
	rm *.out src/*.o