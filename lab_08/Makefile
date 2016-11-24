CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -O2 -std=c11 -I./include
CXX=g++
CXXFLAGS=-Wall -Wextra -Wshadow -O2 -std=c++0x -I./include 
LD=$(CXX)
LDFLAGS=

SUFS=.c .cpp
SRCS=$(foreach suf,$(SUFS),$(wildcard ./src/*$(suf)))
OBJS=$(subst src,bin,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./src/*$(suf)))))
HEADERS=$(wildcard ./include/*.h)
TARGET=matrix

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
