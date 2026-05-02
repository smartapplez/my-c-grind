# Compiler settings
CC = gcc

# Default flags: Warnings + Debug symbols + Basic optimization (or -O0 for no opt)
CFLAGS = -Wall -Wextra -g -O0 

# Optional: Add line tables for better stack traces if you don't want any optimization
DEBUG_FLAGS = -O0 -DDEBUG 

# Input/output files
SRCS = dynamic-arrays.c main.c
OBJS = dynamic-arrays.o main.o
TARGET = main

# Default target: build the standard executable (with debug symbols based on CFLAGS)
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file to an object file
dynamic-arrays.o: dynamic-arrays.c dynamic-arrays.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c dynamic-arrays.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Qwen 3.5 9b:
# DEDICATED DEBUG TARGET
# This adds extra flags often used during development,
# such as enabling macro definitions for DEBUG code.
debug: CFLAGS += -g3 -DDEBUG
debug: $(TARGET)
	# Re-link with debug flags if they changed
	-$(RM) $(OBJS)
	$(CC) $(CFLAGS) -c -o $@ dynamic-arrays.c
	$(CC) $(CFLAGS) -c -o $@ main.c
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) *.tmp

.PHONY: all clean debug

