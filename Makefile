UNAME := $$( uname -s )

.if ${UNAME} == "Linux"
CC=clang --std=gnu99
.else
CC=clang --std=c99
.endif

CFLAGS=-Wall -Wextra -Werror -Weverything -pedantic
LIBS=
BLDFLAGS=-g
RELFLAGS=-O2

TARGET=otc
DEPS=main.c agenda_entry.c date.c
HEAD=agenda_entry.h date.h
OBJ=${DEPS:.c=.o}

.PHONY: all clean release $(TARGET)

all: $(TARGET)

$(OBJ): $(HEAD)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS) $(BLDFLAGS)

release: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LIBS) $(RELFLAGS)

clean:
	rm -f *.o *~ *.core $(OBJ) $(TARGET)
