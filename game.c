#include "game.h"

/** Returns a random int
 * between the given <em>low</em> and <em>high</em> bounds, inclusive.
 */
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

/** Creates all tiles, unshuffled. */
static void init_deck(struct tile deck[TILE_COUNT])
{
	/* Tileset: http://russcon.org/RussCon/carcassonne/tiles.html */
	/* TODO: Refactor so that this fits in 80 columns. */
	size_t ind = 0;
	deck[ind++] = /* start tile, must be first. */
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
			a[i]);
	}
	for (int i = 0; i < 4; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){FIELD, ROAD, ROAD, ROAD, ROAD},
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		const enum attribute a[3] = {NONE, SHIELD, SHIELD};
		deck[ind++] =
			make_tile((enum edge[5]){FIELD, CITY, FIELD, CITY,CITY},
			a[i]);
	}
	for (int i = 0; i < 8; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){ROAD, FIELD, ROAD, FIELD,ROAD},
			NONE);
	}
	for (int i = 0; i < 10; ++i) {
		const enum attribute a[5] = {NONE, NONE, NONE, SHIELD, SHIELD};
		const enum edge b[2] = {ROAD, FIELD};
		deck[ind++] =
			make_tile((enum edge[5]){CITY,b[i%2],b[i%2],CITY,CITY},
			a[i]);
	}
	for (int i = 0; i < 9; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){FIELD, FIELD, ROAD, ROAD,ROAD},
			NONE);
	}
	for (int i = 0; i < 2; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,CITY,FIELD,FIELD,FIELD},
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){FIELD,CITY,FIELD,CITY,FIELD},
			NONE);
	}
	for (int i = 0; i < 2; ++i) {
		deck[ind++] =
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
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,ROAD,ROAD,FIELD,ROAD},
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,FIELD,ROAD,ROAD,ROAD},
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,ROAD,ROAD,ROAD,ROAD},
			NONE);
	}
	for (int i = 0; i < 3; ++i) {
		deck[ind++] =
			make_tile((enum edge[5]){CITY,ROAD,FIELD,ROAD,ROAD},
			NONE);
	}
	assert(ind == TILE_COUNT);
	return;
}

void calculate_scores(struct game *g)
{
	struct feature **scratch = malloc(sizeof(*scratch) * g->features_used);
	size_t len = TILE_COUNT * TILE_COUNT * 4 * 3;
	update_scores(g->scores, scratch, g->features, &len);
	printf("DEBUG: %zu %zu\n", g->scores[0], g->scores[1]);
	free(scratch);
}

void free_game(struct game *g)
{
	struct feature **scratch = malloc(sizeof(*scratch) * g->features_used);
	size_t len = TILE_COUNT * TILE_COUNT * 4 * 3;
	update_scores(g->scores, scratch, g->features, &len);
	for (size_t i = 0; i < len; ++i) {
		free(scratch[i]);
	}
	free(scratch);
}

/** Initialises the given game. */
void make_game(struct game *g)
{
	g->features_used = g->tiles_used = 0;
	for (size_t i = 0; i < PLAYER_COUNT; ++i) {
		g->scores[i] = 0;
		g->meeples[i] = MEEPLE_COUNT;
	}
	init_deck(g->tile_deck);
	/* Leave index 0 untouched. That index will be the starting tile. */
	shuffle_tiles(&g->tile_deck[1], TILE_COUNT - 1);
	g->board = make_board();
	memset(g->features, 0,
			sizeof(g->features[1]) * TILE_COUNT * TILE_COUNT * 4*3);
	return;
}

/** Writes the given deck into the given game's tile_deck by deep copy. */
void set_game_deck(struct game *g, struct tile *deck)
{
	memcpy(g->tile_deck, deck, sizeof(*deck) * TILE_COUNT);
}

/** Tries to play the given move by the player on the given game. */
int play_move(struct game *g, struct move m, int player)
{
	int rc;
	struct slot neighbors[4];
	struct slot *adjs[4];
	for (size_t i = 0; i < 4; ++i) {
		adjs[i] = &neighbors[i];
	}
	if ((rc = play_move_board(&g->board, m, adjs))) {
		return rc;
	}
	if ((rc = play_move_feature(m, adjs, g->features, &g->features_used))) {
		return rc;
	}
	if (m.tcorner > 0 && m.ccorner > 0 ) { /* Either tiger or meeple. */
		return 1;
	}
	if (m.tcorner < 0 && m.ccorner < 0) {
		return rc;
	}
	if ((rc = play_meeple(m, player, g->features))) {
		return rc;
	}
	if (m.tcorner > 0) {
		m.tcorner--;
	}
	if (m.ccorner > 0) {
		m.ccorner--;
	}
	return 0;
}

/** Returns whether the number of tiles dealt for the given game exceeds
 * <em>TILE_COUNT</em>.
 *
 * @see game.h: TILE_COUNT
 */
bool is_tile_deck_empty(struct game *g)
{
	return (TILE_COUNT - (g->tiles_used + 1)) <= 0;
}

/** Returns the next tile from the given game's tile_deck.
 * Unlike peek_current_tile(), \e does advance pointer to next tile.
 */
struct tile deal_tile(struct game *g)
{
	return g->tile_deck[g->tiles_used++];
}

/** Returns the next tile from the given game's tile_deck.
 * Unlike deal_tile(), <em>does not</em> advance pointer to next tile.
 */
inline struct tile peek_current_tile(struct game *g)
{
	return g->tile_deck[g->tiles_used];
}

/**
 * @param out_moves Array to write moves to
 * @param out_moves_len Length of \a out_moves
 * @param g Game to generate possible moveset from
 *
 * @return Number of possible moves supplied into \a out_moves,
 * 	limited by \a out_moves_len.
 *
 * @remarks //TODO
 * 	- Currently does not consider tiger/crocodile placement.
 * 	- If a tile cannot be placed anywhere,
 * 		-# does not indicate that this exceptional event has occurred, and
 * 		-# does not return a set of fallback moves when said event occurs.
 */
int generate_available_moves(struct move* out_moves, int out_moves_len,
	struct game* g)
{
	struct board* b = &g->board;
	struct tile t = peek_current_tile(g);
	struct slot **adjs; // placeholder variable?
	size_t num_possible_moves = 0;

	for(int i = 0; i < b->empty_slot_count; i++){
		// From all possible empty spots...
		struct slot slot_probe = b->slot_spots[i];

		// Test all possible rotations
		for(int rot_probe = 0; rot_probe < 4; rot_probe++){
			struct move move_probe = make_move(t, slot_probe, rot_probe);

			if(invalid_move(*b, move_probe, adjs) == OK){
				out_moves[num_possible_moves++] = move_probe;

				// Check if out_moves can hold anymore moves.
				if(num_possible_moves >= out_moves_len)
					return num_possible_moves; // Quit early, if so
			}
		}
	}

	return num_possible_moves;
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
