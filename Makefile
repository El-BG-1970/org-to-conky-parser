TARGET=otc

CC=clang #--std=c99
CXX=clang #--std=c++11
CFLAGS=-Wall -Wextra -Werror -g
LIBS=
BLDFLAGS=-O2 -g

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
