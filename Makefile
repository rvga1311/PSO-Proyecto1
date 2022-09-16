# Compilador
CC := gcc
# Flags de compilación
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c11 -Wall -lSDL2_ttf -lncurses -lpthread -lSDL2_image $(shell sdl2-config --cflags) -Wall -O
# Archivos.h
HEADERS := frontend.h backend.h
# Archivo main
SOURCES := main.c frontend.c backend.c
# Archivos.o
OBJS := $(SOURCES:.c=.o)
# Nombre del ejecutable
EXEC := main

default: cleanAll compile clean run
	clear

compile: $(EXEC)

$(EXEC): $(OBJS) $(HEADERS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(TFFLAGS)

clean:
	rm -f $(OBJS)

cleanAll:
	rm -f $(EXEC) $(OBJS)

run: $(EXEC)
	clear
	./$(EXEC)