CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -O2 -std=c11 -I./include

SRCS=$(wildcard ./src/*.c)
OBJS=$(foreach s,$(SRCS:.c=.o),$(subst src,bin,$(s)))
HEADERS=$(wildcard ./include/*.h)
TARGET=pose_db

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -Rf bin $(TARGET)

.PHONY: all clean
