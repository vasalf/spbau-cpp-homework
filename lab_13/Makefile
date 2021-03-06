CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -Wimplicit -O2 -std=c11 -I./include
CXX=g++
CXXFLAGS=-Wall -Wextra -Wshadow -Werror -pedantic -std=c++14 -O2 -I./include
LD=$(CXX)
LDFLAGS=
TEST_LDFLAGS=$(LDFLAGS) -lgtest -lpthread

SUFS=.c .cpp
SRCS=$(foreach suf,$(SUFS),$(wildcard ./src/*$(suf)))
OBJS=$(subst src,bin,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./src/*$(suf)))))
HEADERS=$(wildcard ./src/*.h)
TARGET=main

TEST_SRCS=$(foreach suf,$(SUFS),$(wildcard ./test/*$(suf)))
TEST_OBJS=$(subst test/,bin/,$(foreach suf,$(SUFS),$(subst $(suf),.o,$(wildcard ./test/*$(suf))))) 
TEST_HEADERS=$(wildcard ./test/*.h)
TEST_TARGET=test_array

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET)

bin:
	mkdir -p bin

bin/%.o: src/%.c Makefile $(HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.cpp Makefile $(HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_OBJS)
	$(LD) $(TEST_OBJS) $(TEST_LDFLAGS) -o $(TEST_TARGET)

bin/%.o: test/%.c Makefile $(TEST_HEADERS) | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: test/%.cpp Makefile $(TEST_HEADERS) | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -Rf bin $(TARGET) $(TEST_TARGET)

.PHONY: all $(TEST_TARGET) clean
