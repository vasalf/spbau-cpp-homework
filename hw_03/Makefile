CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -O2 -std=c11 -I./src
CXX=g++
CXXFLAGS=-Wall -Wextra -Wshadow -Werror -pedantic -std=c++14 -g -I./src
LD=$(CXX)
LDFLAGS=

SUFS=.c .cpp
SRCS=$(foreach suf,$(SUFS),$(wildcard ./src/*$(suf)))
OBJS=$(subst src,bin,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./src/*$(suf)))))
HEADERS=$(wildcard ./src/*.h)
TARGET=huffman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET)

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.cpp Makefile $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -Rf bin $(TARGET)

.PHONY: all clean
