
UNAME_S := $(shell uname -s 2>/dev/null)

CC      ?= cc
CFLAGS  ?= -Wall -Wextra -Wpedantic -g3
LDFLAGS ?=

SRC    = src/main.c src/parser.c src/executor.c src/path_utils.c src/builtins.c src/get_line.c
OBJ    = $(SRC:.c=.o)
TARGET = hsh

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)
