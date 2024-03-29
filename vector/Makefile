CC              := g++-12
CFLAGS          := -std=c++20
CFLAGS_SANITIZE := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

.PHONY: all
all: build run

.PHONY: build
build:
	$(CC) $(CFLAGS) $(CFLAGS_SANITIZE) vector.cpp -o vector

.PHONY: run
run:
	./vector > log.txt

.PHONY: clean
clean:
	rm -f vector
	rm -f log.txt
