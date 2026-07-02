CC = gcc
CFLAGS = -Wall -Wextra -O3 -g

SRCS = circular_buffer.c main.c
OBJS = circular_buffer.o main.o
TARGET = circular_buffer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

circular_buffer.o: circular_buffer.c circular_buffer.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c circular_buffer.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET) *.tmp

.PHONY: all clean
