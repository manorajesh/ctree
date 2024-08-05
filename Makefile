CC = clang
CFLAGS = -Wall -Wextra -Iinclude -O3
OBJ = src/main.o
TARGET = ctree

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)