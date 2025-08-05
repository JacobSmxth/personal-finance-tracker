CC=gcc
CFLAGS=-Wall -Werror -Wextra -g

SRC=src/main.c
OUT=app

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)


run:
	./$(OUT)

edit:
	nvim ./$(SRC)

clear:
	rm -rf $(OUT)
