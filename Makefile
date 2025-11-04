CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -I./include

SRC = src/main.c src/mapa.c src/jogador.c src/bala.c
LIBSRC = cli-lib/src/*.c
BIN = TheLastMan

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBSRC) -o $(BIN)

clean:
	rm -f $(BIN)
