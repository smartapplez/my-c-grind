CC = gcc
CFLAGS = -Wall -Wextra -g -O0

DEBUG_FLAGS = -O0 -DDEBUG

SRCS = my-stack.c main.c
OBJS = my-stack.o main.o
TARGET = stack-test

# Default target: build the executable
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

# Compile each source file to an object file
my-stack.o: my-stack.c my-stack.h
	$(CC) $(CCFLAGS) -c -o $@ $<

main.o: main.c my-stack.h
	$(CC) $(CCFLAGS) -c -o $@ $<
		
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

#TODO: Add Debug flags so that I can see error logs and valgrind
