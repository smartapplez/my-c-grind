# Compiler Settings
CC = gcc

# Compiler FLags
CFLAGS = -Wall -Wextra -O0 -g

# All the sources that will be compiled
SRCS = davinci_video_convert.c

# The Source files that will be utilized
OBJS = davinci_video_convert.o

# Target binary file name
TARGET = davinciconv

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

davinci_video_convert.o: davinci_video_convert.c davinci_video_convert.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -f $(OBJS) $(TARGET) *.tmp

run: all
	./$(TARGET) -v


.PHONY: all clean run
