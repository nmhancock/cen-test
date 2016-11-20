#include "game.h"

static size_t rand_bound(size_t low, size_t high)
{
	size_t r;
	do {
		r = round(genrand64_real1());
	} while (r < low || r > high);
	return r;
}

/* Modern Fisher-Yates per Wikipedia.
 * /wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
*/
static void shuffle_tiles(struct tile *a, size_t top)
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	init_genrand64(tp.tv_nsec); /* Seed the PRNG for the shuffling. */

	for (size_t i = top - 1; i > 1; --i) {
		size_t j = rand_bound(0, i);
		struct tile swap = a[i];
		a[i] = a[j];
		a[j] = swap;
	}
}

static void init_deck(struct tile deck[TILE_COUNT])
{
	/* Tileset: http://russcon.org/RussCon/carcassonne/tiles.html */
	/* TODO: Refactor so that this fits in 80 columns. */
	size_t ind = 0;
	deck[ind++] = /* start tile, must be first. */
<<<<<<< HEAD
		make_tile((enum edge[5]){LAKE, GAME-TRAIL, JUNGLE, GAME-TRAIL, GAME-TRAIL},
		NONE);
	deck[ind++] =
		make_tile((enum edge[5]){LAKE, LAKE, LAKE, LAKE, LAKE}, SHIELD);
	deck[ind++] =
		make_tile((enum edge[5]){GAME-TRAIL, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL}, NONE);
	for (int i = 0; i < 7; ++i) {
		const enum attribute a[7] =
			{NONE, NONE, NONE, SHIELD, NONE, SHIELD, SHIELD};
		const enum edge b[7] = {JUNGLE,JUNGLE,JUNGLE,JUNGLE,GAME-TRAIL,GAME-TRAIL,GAME-TRAIL};
		deck[ind++] =
			make_tile((enum edge[5]){LAKE, LAKE, b[i], LAKE, LAKE},
=======
		make_tile((enum edge[5]){CITY, ROAD, FIELD, ROAD, ROAD},
		NONE);
	deck[ind++] =
		make_tile((enum edge[5]){CITY, CITY, CITY, CITY, CITY}, SHIELD);
	deck[ind++] =
		make_tile((enum edge[5]){ROAD, ROAD, ROAD, ROAD, ROAD}, NONE);
	for (int i = 0; i < 7; ++i) {
		const enum attribute a[7] =
			{NONE, NONE, NONE, SHIELD, NONE, SHIELD, SHIELD};
		const enum edge b[7] = {FIELD,FIELD,FIELD,FIELD,ROAD,ROAD,ROAD};
		deck[ind++] =
			make_tile((enum edge[5]){CITY, CITY, b[i], CITY, CITY},
>>>>>>> Nick495/dev
			a[i]);
	}
	for (int i = 0; i < 4; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){JUNGLE, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL},
=======
			make_tile((enum edge[5]){FIELD, ROAD, ROAD, ROAD, ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		const enum attribute a[3] = {NONE, SHIELD, SHIELD};
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){JUNGLE, LAKE, JUNGLE, LAKE,LAKE},
=======
			make_tile((enum edge[5]){FIELD, CITY, FIELD, CITY,CITY},
>>>>>>> Nick495/dev
			a[i]);
	}
	for (int i = 0; i < 8; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){GAME-TRAIL, JUNGLE, GAME-TRAIL, JUNGLE,GAME-TRAIL},
=======
			make_tile((enum edge[5]){ROAD, FIELD, ROAD, FIELD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 10; ++i) {
		const enum attribute a[5] = {NONE, NONE, NONE, SHIELD, SHIELD};
<<<<<<< HEAD
		const enum edge b[2] = {GAME-TRAIL, JUNGLE};
		deck[ind++] =
			make_tile((enum edge[5]){LAKE,b[i%2],b[i%2],LAKE,LAKE},
=======
		const enum edge b[2] = {ROAD, FIELD};
		deck[ind++] =
			make_tile((enum edge[5]){CITY,b[i%2],b[i%2],CITY,CITY},
>>>>>>> Nick495/dev
			a[i]);
	}
	for (int i = 0; i < 9; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){JUNGLE, JUNGLE, GAME-TRAIL, GAME-TRAIL,GAME-TRAIL},
=======
			make_tile((enum edge[5]){FIELD, FIELD, ROAD, ROAD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 2; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){LAKE,LAKE,JUNGLE,JUNGLE,JUNGLE},
=======
			make_tile((enum edge[5]){CITY,CITY,FIELD,FIELD,FIELD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){JUNGLE,LAKE,JUNGLE,LAKE,JUNGLE},
=======
			make_tile((enum edge[5]){FIELD,CITY,FIELD,CITY,FIELD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 2; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){JUNGLE,JUNGLE,GAME-TRAIL,JUNGLE,JUNGLE},
			DEN);
	}
	for (int i = 0; i < 4; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){JUNGLE,JUNGLE,JUNGLE,JUNGLE,JUNGLE},
			DEN);
	}
	for (int i = 0; i < 5; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){LAKE,JUNGLE,JUNGLE,JUNGLE,JUNGLE},
=======
			make_tile((enum edge[5]){FIELD,FIELD,ROAD,FIELD,FIELD},
			MONASTERY);
	}
	for (int i = 0; i < 4; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){FIELD,FIELD,FIELD,FIELD,FIELD},
			MONASTERY);
	}
	for (int i = 0; i < 5; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,FIELD,FIELD,FIELD,FIELD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){LAKE,GAME-TRAIL,GAME-TRAIL,JUNGLE,GAME-TRAIL},
=======
			make_tile((enum edge[5]){CITY,ROAD,ROAD,FIELD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){LAKE,JUNGLE,GAME-TRAIL,GAME-TRAIL,GAME-TRAIL},
=======
			make_tile((enum edge[5]){CITY,FIELD,ROAD,ROAD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){LAKE,GAME-TRAIL,GAME-TRAIL,GAME-TRAIL,GAME-TRAIL},
=======
			make_tile((enum edge[5]){CITY,ROAD,ROAD,ROAD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
<<<<<<< HEAD
			make_tile((enum edge[5]){LAKE,GAME-TRAIL,JUNGLE,GAME-TRAIL,GAME-TRAIL},
=======
			make_tile((enum edge[5]){CITY,ROAD,FIELD,ROAD,ROAD},
>>>>>>> Nick495/dev
			NONE);
	}
	assert(ind == TILE_COUNT);
	return;
}

void make_game(struct game *g)
{
	g->graphs_used = g->tiles_used = g->scores[0] = g->scores[1] = 0;
	init_deck(g->tile_deck);
	/* The first index must be 0 (have to start with start tile). */
	shuffle_tiles(&g->tile_deck[1], TILE_COUNT - 1);
	g->board = make_board();
	return;
}

void make_game_with_deck(struct game *g, struct tile *deck)
{
	memcpy(g->tile_deck, deck, sizeof(*deck) * TILE_COUNT);
}

int play_move(struct game *g, struct move m, int player)
{
	return play_move_board(&g->board, m);
	// Graph and score stuff here.
}

<<<<<<< HEAD
=======
int more_tiles(struct game *g)
{
	return TILE_COUNT - g->tiles_used - 1;
}

>>>>>>> Nick495/dev
struct tile deal_tile(struct game *g)
{
	return g->tile_deck[g->tiles_used++];
}

#ifdef TEST
int main(void)
{
	struct game g;
	make_game(&g);
	char buf[TILE_LEN];
	for (int i = 0; i < TILE_COUNT; ++i) {
		printf("%s\n", print_tile(deal_tile(&g), buf));
	}
	return 0;
}
#endif
