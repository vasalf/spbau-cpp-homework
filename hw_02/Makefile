CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -O2 -std=c11 -I./include
CXX=g++
CXXFLAGS=-Wall -Wextra -Wshadow -O2 -std=c++0x -g -I./include 
LD=$(CXX)
LDFLAGS=

SUFS=.c .cpp
SRCS=$(foreach suf,$(SUFS),$(wildcard ./src/*$(suf)))
OBJS=$(subst src,bin,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./src/*$(suf)))))
TEST_SRCS=$(foreach suf,$(SUFS),$(wildcard ./test/*$(suf)))
TEST_OBJS=$(subst ./bin/main.o,,$(OBJS)) $(subst test/,bin/,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./test/*$(suf)))))
HEADERS=$(wildcard ./include/*.h)
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET)

board_test: $(TEST_OBJS)
	$(LD) $(TEST_OBJS) $(LDFLAGS) -o board_test

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: test/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.cpp Makefile $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/%.o: test/%.cpp Makefile $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -Rf bin $(TARGET) board_test

.PHONY: all clean
