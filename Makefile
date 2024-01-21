# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Linker flags (including -lm for the math library)
LDFLAGS = -lm

# Source files
SRC = mvote.c parsing.c hash_table.c inverted_list.c

# Header files
HEADERS = parsing.h hash_table.h inverted_list.h voter.h

# Object files
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = mvote

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

