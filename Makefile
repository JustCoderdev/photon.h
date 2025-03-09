# JustCoderdev Makefile for C projects v6

CORE_FILES =

CC = gcc
CCFLAGS = -xc -std=c89 -ansi -pedantic-errors -pedantic \
		 -Wall -Wextra -Werror -Wshadow -Wpointer-arith \
		 -Wcast-qual -Wcast-align -Wstrict-prototypes \
		 -Wmissing-prototypes -Wconversion -g

IFLAGS = -I./ -I./lib/include -I../include
LDFLAGS = -L./ -lGL -lglfw -lm

DFLAGS = -DDEBUG_ENABLE=1
FLAGS = $(CCFLAGS) $(IFLAGS) $(LDFLAGS) $(DFLAGS)

.PHONY: all
.PHONY: bin
.PHONY: clean

all:
all-tests: bin/open

bin/opengl-test: bin opengl.c
	@echo "Compiling... (opengl-test)"
	$(CC) $(FLAGS) opengl.c -o bin/opengl-test

bin:
	@mkdir -p bin

clean:
	@echo -n "Cleaning... "
	@rm -rf ./bin
	@echo "Done"
