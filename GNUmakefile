CC=gcc

CFLAGS=-Wall -Wextra -Werror -Wpadded -pedantic
LIBS=
BLDFLAGS=-O2 -g

TARGET=otc
DEPS=main.c agenda_entry.c date.c
HEAD=agenda_entry.h date.h
OBJ=${DEPS:.c=.o}

.PHONY: all clean $(TARGET)

all: $(TARGET)

$(OBJ): $(HEAD)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS) $(BLDFLAGS)

clean:
	rm -f *.o *~ *.core $(OBJ) $(TARGET)
