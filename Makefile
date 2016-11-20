<<<<<<< HEAD
<<<<<<< HEAD
CFLAGS=-std=c99 -flto -Wall -Wextra -pedantic -O2
all: board.c tile.o slot.o
	$(CC) $(CFLAGS) -o test board.c tile.o slot.o
=======
CFLAGS=-std=c99 -g -march=native -flto -Wall -Wextra -pedantic -O2
=======
CFLAGS=-std=c99 -g -march=native -flto -Wall -Wextra -pedantic -O0
>>>>>>> Nick495/dev

all: game board server client

clean:
	rm *.o

<<<<<<< HEAD
server: server.c game.o rng.o tile.o board.o slot.o
	$(CC) $(CFLAGS) -o server server.c game.o rng.o tile.o board.o slot.o \
		-lm -pthread

client: client.c game.o rng.o tile.o board.o slot.o
	$(CC) $(CFLAGS) -o client client.c game.o rng.o tile.o board.o slot.o \
		-lm
=======
server: server.c game.o rng.o tile.o board.o slot.o serialization.o
	$(CC) $(CFLAGS) -o server server.c game.o rng.o tile.o move.o board.o \
		slot.o serialization.o -lm -pthread

client: client.c game.o rng.o tile.o board.o slot.o serialization.o
	$(CC) $(CFLAGS) -o client client.c game.o rng.o tile.o move.o board.o \
		slot.o serialization.o -lm
>>>>>>> Nick495/dev

game: game.c game.h rng.o tile.o board.o slot.o
	$(CC) $(CFLAGS) -DTEST -o test_game game.c rng.o tile.o board.o slot.o \
		-lm

board: board.c board.h tile.o slot.o move.o
	$(CC) $(CFLAGS) -DTEST -o test_board board.c tile.o slot.o move.o

<<<<<<< HEAD
=======
serialization.o: serialization.c serialization.h
	$(CC) $(CFLAGS) -c -o serialization.o serialization.c
>>>>>>> Nick495/dev
game.o: game.c game.h
	$(CC) $(CFLAGS) -c -o game.o game.c

board.o: board.c board.h tile.o slot.o move.o
	$(CC) $(CFLAGS) -c -o board.o board.c

rng.o: rngs/mt19937-64.c rngs/mt19937-64.h
	$(CC) $(CFLAGS) -c -o rng.o rngs/mt19937-64.c

move.o: move.c move.h
	$(CC) ${CFLAGS} -c -o move.o move.c

<<<<<<< HEAD
>>>>>>> origin/dev
=======
>>>>>>> Nick495/dev
tile.o: tile.c tile.h
	$(CC) ${CFLAGS} -c -o tile.o tile.c

slot.o: slot.c slot.h
	$(CC) ${CFLAGS} -c -o slot.o slot.c
