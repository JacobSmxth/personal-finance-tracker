CC=gcc
CFLAGS=-Wall -Werror -Wextra -g
MFLAGS=-largon2 -o

SRC=src/main.c
OUT=app

all:
	$(CC) $(SRC) $(MFLAGS) $(OUT) $(CFLAGS)


run:
	./$(OUT)

edit:
	nvim ./$(SRC)

clear:
	rm -rf $(OUT)
