CC = gcc
# CFLAGS = -Wall -Wextra -g -O2
CFLAGS = -Wall -Wextra -g -O2
# MAX_BUF_SIZE_FLAG = -DMAX_BUFFER_SIZE=8

DEBUG_FLAGS = -O0 -DDEBUG

SRCS = linked-list.c main.c node.c
OBJS = linked-list.o main.o node.o
TARGET = linked-list-test

# Default target: build the executable
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

# Compile each source file to an object file
linked-list.o: linked-list.c linked-list.h node.h node.c
	$(CC) $(CCFLAGS) -c -o $@ $<

node.o: node.c node.h
	$(CC) $(CCFLAGS) -c -o $@ $<

main.o: main.c linked-list.h node.h
	$(CC) $(CCFLAGS) -c -o $@ $<
		
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
