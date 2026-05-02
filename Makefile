# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Input/output files
SRCS = dynamic-arrays.c main.c
OBJS = dynamic-arrays.o main.o
TARGET = main

# Default target: build the executable
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file to an object file
dynamic-arrays.o: dynamic-arrays.c dynamic-arrays.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c dynamic-arrays.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets for convenience
.PHONY: all clean

