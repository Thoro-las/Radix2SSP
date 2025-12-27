CC = gcc
CFLAGS = -lm -w -g

all: run

run: main
	@clear
	@echo ========================================
	@./main

main: src/*
	$(CC) src/main.c -o main $(CFLAGS)

clean:
	rm -f main