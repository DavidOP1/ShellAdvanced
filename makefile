CC = gcc
CFLAGS = -w

SRC = myshell.c
OBJ = $(SRC:.c=.o)
EXEC = myshell

.PHONY: all clean run

all: $(EXEC) run

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)