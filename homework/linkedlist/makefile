cc = gcc
TARGET = linkedlist.elf

SRCS = intlist.c main.c
HDRS = intlist.h

OBJS = $(SRCS: .c=.o)

%.o: %.c $(HDRS)
	$(CC) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)