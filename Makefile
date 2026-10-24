CC := gcc
LIBS := -lraylib
CFLAGS := -Wall -Wextra -pedantic -std=c23 -g ${LIBS}

main: main.c
	${CC} ${CFLAGS} $@.c -o $@

run: main
	./main

debug: main
	gdb ./main

clean: main
	rm main

