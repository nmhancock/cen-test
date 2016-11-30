#ifndef GAME_H_
#define GAME_H_

/* clock_gettime() */
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */
#include <sys/time.h>
#include <time.h>

#include <stddef.h>	/* size_t */
#include <math.h>	/* round() */
#include <assert.h>	/* assert() */
#include <pthread.h>

#include "limits.h"
#include "tile.h"
#include "board.h"
#include "feature.h"
#include "rngs/mt19937-64.h" /* Mersenne Twister PRNG. Try PCG if too slow */

#define MEEPLE_COUNT 7
#define PLAYER_COUNT 2

struct game {
	struct board board;
	struct tile tile_deck[TILE_COUNT];
	size_t tiles_used;
	size_t scores[PLAYER_COUNT];
	size_t meeples[PLAYER_COUNT];
	struct feature *features[AXIS * AXIS * 4 * 3];
	size_t features_used;
};

void calculate_scores(struct game *g);
void make_game(struct game *g);
void free_game(struct game *g);
void set_game_deck(struct game *g, struct tile *deck);
int play_move(struct game *g, struct move m, int player);
int is_move_valid(struct game *g, struct move m, int player);
bool is_tile_deck_empty(struct game *g);
struct tile deal_tile(struct game *g);
void generate_available_moves(struct game *g, int player, \
		struct tile t, struct move *pmoves, size_t *pmoves_len);
#endif
