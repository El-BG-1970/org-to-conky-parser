UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
CC=clang --std=gnu99
CXX=clang --std=gnu++11
else
CC=clang --std=c99
CXX=clang --std=c++11
endif

CFLAGS=-Wall -Wextra -Werror -g
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
