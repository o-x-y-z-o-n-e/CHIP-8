COMPILER = gcc

OBJECTS = src/main.c src/core.c src/decode.c src/video.c

COMPLIER_FLAGS = -w

LINKER_FLAGS = -l SDL2

OUTPUT = bin/linux/chip_8

all : $(OBJECTS)
	$(COMPILER) $(OBJECTS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OUTPUT)
