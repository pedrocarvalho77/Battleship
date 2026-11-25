CC     = gcc
CFLAGS = -Wall -std=c99 -Iinclude
SRCS   = $(filter-out src/engine.c, $(wildcard src/*.c))
OBJS   = $(SRCS:src/%.c=build/%.o)

.PHONY: all run ui run-ui clean

all: build/engine

run: all
	./build/engine

build/engine: src/engine.c build/libgame.a
	$(CC) $(CFLAGS) src/engine.c -o $@ -Lbuild -lgame

# graphical version, needs raylib (brew install raylib)
ui: build/battleship-ui

run-ui: ui
	./build/battleship-ui

build/battleship-ui: ui/main.c build/libgame.a
	$(CC) $(CFLAGS) $(shell pkg-config --cflags raylib) ui/main.c -o $@ -Lbuild -lgame $(shell pkg-config --libs raylib)

build/libgame.a: $(OBJS)
	ar -rc $@ $(OBJS)

build/%.o: src/%.c $(wildcard include/*.h) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build
