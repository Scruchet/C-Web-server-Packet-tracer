CC = gcc

CFLAGS = -Wall -g -Os -I../libs/Reseau
LDFLAGS= -L../libs/Reseau
LIBS = -lreseau

TARGET = sioux
SOURCES = $(wildcard *.c)
SOURCES_H = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f *.o $(TARGET) *.csv




format:
	clang-format -i -style="WebKit" $(SOURCES) $(SOURCES_H)
