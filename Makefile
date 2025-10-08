# JustCoderdev Makefile for C projects v6

CORE_FILES =
PHOTON_FILES = lib/photon/utils.c bin/pedantic-workarounds.o
LIB_HEADERS  = lib/include/photon.h lib/include/photon-runner.h lib/include/photon-common.h

CC = gcc
CCFLAGS = -xc -std=c89 -ansi -pedantic-errors -pedantic \
		 -Wall -Wextra -Werror -Wshadow -Wpointer-arith \
		 -Wcast-qual -Wcast-align -Wstrict-prototypes \
		 -Wmissing-prototypes -Wconversion -g \
		 -Wno-unused-variable -Wfatal-errors


IFLAGS = -I./ -I./lib/include
LDFLAGS = -L./ -L./bin -lGL -lglfw -lm -Wl,-rpath=./bin

DFLAGS = -DDEBUG_ENABLE=1
FLAGS = $(CCFLAGS) $(IFLAGS) $(LDFLAGS) $(DFLAGS)

.PHONY: all
.PHONY: bin
.PHONY: clean

all: bin/gas.so bin/engine

bin/pedantic-workarounds.o: bin lib/photon/pedantic-workarounds.c
	@echo "Compiling... (pedantic-workarounds.o)"
	$(CC) $(FLAGS) -Wno-pedantic -c lib/photon/pedantic-workarounds.c
	@mv *.o bin

bin/engine: bin engine.c $(PHOTON_FILES) $(LIB_HEADERS)
	@echo "Compiling... (engine)"
	$(CC) $(FLAGS) engine.c $(PHOTON_FILES) -o $@
	# -l:gas.so

bin/gas.so: bin gas.c $(PHOTON_FILES) $(LIB_HEADERS)
	@echo "Compiling... (gas)"
	$(CC) $(FLAGS) -fPIC -shared gas.c -o $@

bin:
	@mkdir -p bin

clean:
	@echo -n "Cleaning... "
	@rm -rf ./bin
	@echo "Done"
