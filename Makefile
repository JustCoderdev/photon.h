# JustCoderdev Makefile for C projects v6

CORE_FILES =
PHOTON_FILES = bin/obj/utils.o bin/obj/pedantic-workarounds.o
LIB_HEADERS  = lib/include/photon.h lib/include/photon-runner.h lib/include/photon-common.h

CC = gcc
CCFLAGS = -std=c89 -ansi -pedantic-errors -pedantic \
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

all: bin/engine bin/gas.so


#####


bin/obj/utils.o: bin lib/photon/utils.c
	@echo "Compiling... (utils.o)"
	$(CC) $(FLAGS) lib/photon/utils.c -o $@ -c

bin/obj/pedantic-workarounds.o: bin lib/photon/pedantic-workarounds.c
	@echo "Compiling... (pedantic-workarounds.o)"
	$(CC) $(FLAGS) -Wno-pedantic lib/photon/pedantic-workarounds.c -o $@ -c

bin/obj/engine.o: bin engine.c
	@echo "Compiling... (engine.o)"
	$(CC) $(FLAGS) engine.c -o $@ -c

bin/obj/gas.o: bin gas.c
	@echo "Compiling... (gas.o)"
	$(CC) $(FLAGS) gas.c -o $@ -c


#####

bin/engine: bin bin/obj/engine.o $(PHOTON_FILES)
	@echo "Compiling... (engine)"
	$(CC) $(FLAGS) bin/obj/engine.o $(PHOTON_FILES) -o $@
	# -l:gas.so

bin/gas.so: bin bin/obj/gas.o $(PHOTON_FILES)
	@echo "Compiling... (gas)"
	$(CC) $(FLAGS) -fPIC -shared bin/obj/gas.o $(PHOTON_FILES) -o $@


#####

bin:
	@mkdir -p bin/obj

clean:
	@echo -n "Cleaning... "
	@rm -rf ./bin
	@echo "Done"
