CFLAGS_RELEASE := -fno-elide-constructors -O0
CFLAGS_DEBUG   := -D _DEBUG -std=c++20 -ggdb3 -Wall -O0 -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -Wstack-usage=8192 -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CFLAGS := $(CFLAGS_RELEASE)

.PHONY: all
all: build run

.PHONY: build
build:
	mkdir -p img/
	g++ $(CFLAGS) main.cpp log_int.cpp -o log_int

.PHONY: run
run:
	./log_int

.PHONY: clean
clean:
	rm -f log_int
	rm -f log.dot
