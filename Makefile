main: main.c
	$(CC) *.c libs/*.c -o out/main -lSDL2 -lSDL2_Image -Wall -Wextra -pedantic -std=c99
