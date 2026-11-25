CC     = gcc
CFLAGS = -Wall -std=c99 -Iinclude
SRCS   = $(filter-out src/engine.c, $(wildcard src/*.c))
OBJS   = $(SRCS:src/%.c=build/%.o)

.PHONY: all run clean

all: build/engine

run: all
	./build/engine

build/engine: src/engine.c build/libgame.a
	$(CC) $(CFLAGS) src/engine.c -o $@ -Lbuild -lgame

build/libgame.a: $(OBJS)
	ar -rc $@ $(OBJS)

build/%.o: src/%.c $(wildcard include/*.h) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build
