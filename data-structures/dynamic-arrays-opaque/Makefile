# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -O0

DEBUG_FLAGS = -O0 -DDEBUG

# Input/output files
SRCS = dynamic-arrays-opaque.c main.c
OBJS = dynamic-arrays-opaque.o main.o
TARGET = main

# Default target: build the executable
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file to an object file
dynamic-arrays-opaque.o: dynamic-arrays-opaque.c dynamic-arrays-opaque.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c dynamic-arrays-opaque.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets for convenience
.PHONY: all clean

# TODO: Add Debug Target for dynamic-arrays-opaque
