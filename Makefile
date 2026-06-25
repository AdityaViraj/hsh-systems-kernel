CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -g3
SRC = src/main.c src/parser.c src/executor.c src/path_utils.c src/builtins.c src/get_line.c
OBJ = $(SRC:.c=.o)
TARGET = hsh

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)