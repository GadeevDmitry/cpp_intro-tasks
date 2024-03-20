CC     := g++
CFLAGS := -std=c++20

.PHONY: all
all: build run

.PHONY: build
build:
	$(CC) $(CFLAGS) function.cpp -o function

.PHONY: run
run:
	./function

.PHONY: clean
clean:
	rm -f function
