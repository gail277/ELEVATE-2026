CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror
TARGET = linkedlist.elf

SRCS = intlist.c main.c
HDRS = intlist.h

OBJS = $(SRCS:.c=.o)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)


clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: lint docs

lint:
	splint $(SRCS)

docs:
	doxygen Doxyfile