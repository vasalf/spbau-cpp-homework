CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -I./include

SRCS=$(wildcard ./src/*.c)
OBJS=$(foreach s,$(SRCS:.c=.o),$(subst src,bin,$(s)))

all: lab1

lab1: $(OBJS)
	$(CC) $(OBJS) -o lab1

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -Rf bin lab1

.PHONY: all clean
