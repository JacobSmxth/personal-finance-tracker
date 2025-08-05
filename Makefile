CC=gcc
CFLAGS=-Wall -Werror -Wextra -g
LDFLAGS=-largon2 -lsodium

SRC=src/main.c
OUT=app

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)


run:
	./$(OUT)

edit:
	nvim ./$(SRC)

maker:
	nvim ./Makefile

clear:
	rm -rf $(OUT)
