CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -I./include

SRCS=$(wildcard ./src/*.c)
OBJS=$(foreach s,$(SRCS:.c=.o),$(subst src,bin,$(s)))
HEADERS=$(wildcard ./include/*.h)

all: lab1

lab1: $(OBJS)
	$(CC) $(OBJS) -o lab1

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -Rf bin lab1

.PHONY: all clean
