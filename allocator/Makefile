CC              := clang++
CFLAGS          := -std=c++20 -I../include/
CFLAGS_SANITIZE := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

.PHONY: all
all: build run

.PHONY: build
build:
	$(CC) $(CFLAGS) $(CFLAGS_SANITIZE) allocator.cpp -o allocator

.PHONY: run
run:
	./allocator

.PHONY: clean
clean:
	rm -f allocator
