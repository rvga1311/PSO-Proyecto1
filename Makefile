# Compilador
CC := gcc
# Flags de compilación
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image $(shell sdl2-config --cflags) -Wall -O
# Archivos.h
HEADERS :=
# Archivo main
SOURCE := main.c
# Archivos.o
OBJS := $(SOURCE:.c=.o)
# Nombre del ejecutable
EXEC := main

default: cleanAll compile clean run
	clear

compile: $(EXEC)

$(EXEC): $(OBJS) $(HEADERS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(OBJS)

cleanAll:
	rm -f $(EXEC) $(OBJS)

run: $(EXEC)
	clear
	./$(EXEC)