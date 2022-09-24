# Compilador
CC := gcc
# Flags de compilación
SLD2_FLAGS := `sdl2-config --libs --cflags` $(shell sdl2-config --cflags) -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CFLAGS :=  -ggdb3 -O0 --std=c11 -Wall -lpthread -Wall -O
# Archivos.h
HEADERS := frontend.h backend.h
# Archivo main
SOURCES := main.c frontend.c backend.c
# Archivos.o
OBJS := $(SOURCES:.c=.o)
# Nombre del ejecutable
EXEC := Game

default: cleanAll compile clean run
	clear

compile: $(EXEC)

$(EXEC): $(OBJS) $(HEADERS) Makefile
	$(info Compiling... )
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(SLD2_FLAGS)  $(TFFLAGS)

clean:
	$(info Removing .o files... )
	rm -f $(OBJS)

cleanAll:
	$(info Removing .o and executable files ...)
	rm -f $(EXEC) $(OBJS)

run: $(EXEC)
	clear
	$(info Running $(EXEC))
	./$(EXEC)

install_SDL2:
	$(info Installing required dependencies for SDL2...)
	sudo apt-get update -y
	sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-image-2.0-0 libsdl-ttf2.0-0 libsdl2-mixer-2.0-0