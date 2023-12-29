CC = gcc

CFLAGS = -W -g -Os -lpcap

TARGET = ablette
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
all: $(TARGET)


$(TARGET):
	$(CC) -o $(TARGET) $(TARGET).c $(CFLAGS)
.PHONY: clean

clean:
	rm -f *.o $(TARGET)

start :
	sudo ./$(TARGET)


format:
	clang-format -i -style="WebKit" $(SOURCES)
