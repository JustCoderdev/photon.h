# JustCoderdev Makefile for C projects v6

CORE_FILES =
PHOTON_FILES = lib/photon/utils.c
LIB_HEADERS  = lib/include/photon.h

CC = gcc
CCFLAGS = -xc -std=c89 -ansi -pedantic-errors -pedantic \
		 -Wall -Wextra -Werror -Wshadow -Wpointer-arith \
		 -Wcast-qual -Wcast-align -Wstrict-prototypes \
		 -Wmissing-prototypes -Wconversion -g \
		 -Wno-unused-variable

IFLAGS = -I./ -I./lib/include
LDFLAGS = -L./ -lGL -lglfw -lm

DFLAGS = -DDEBUG_ENABLE=1
FLAGS = $(CCFLAGS) $(IFLAGS) $(LDFLAGS) $(DFLAGS)

.PHONY: all
.PHONY: bin
.PHONY: clean

all:

bin/engine: bin engine.c $(PHOTON_FILES) $(LIB_HEADERS)
	@echo "Compiling... (engine)"
	$(CC) $(FLAGS) engine.c $(PHOTON_FILES) -o bin/engine

bin/gas: bin gas.c $(PHOTON_FILES) $(LIB_HEADERS)
	@echo "Compiling... (gas)"
	$(CC) $(FLAGS) gas.c -o bin/gas

bin:
	@mkdir -p bin

clean:
	@echo -n "Cleaning... "
	@rm -rf ./bin
	@echo "Done"
