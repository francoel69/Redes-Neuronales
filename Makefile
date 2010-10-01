CC = gcc
CFLAGS = -Wall -Wextra -Wdeclaration-after-statement -Wbad-function-cast \
	-Wstrict-prototypes -Wmissing-declarations \
	-Wmissing-prototypes -Werror -g -O2
LIBS = -lm
TARGET = red
#SOURCES = red_hopfield_deterministav04.c matrix.c ran2.c
SOURCES = red_hopfield_estocastica.c matrix.c ran2.c
OBJECTS = $(SOURCES:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

-include .depend

.depend: $(SOURCES)
	$(CC) -MM $(SOURCES) > .depend

.PHONY: clean

clean:
	rm -f $(TARGET) *.o .depend *~
