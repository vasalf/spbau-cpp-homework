CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -g -D_GLIBCXX_DEBUG -std=c11 -I./include
LDFLAGS=-lexpat

SRCS=$(wildcard ./src/*.c)
OBJS=$(foreach s,$(SRCS:.c=.o),$(subst src,bin,$(s)))
HEADERS=$(wildcard ./include/*.h)
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -Rf bin $(TARGET)

.PHONY: all clean
