#include "board.h"

<<<<<<< HEAD
<<<<<<< HEAD
#define AXIS 5 /* AXIS by AXIS board */

struct board {
	struct tile tiles[AXIS*AXIS];
	struct slot slot_spots[AXIS*AXIS];
	unsigned int sps; /* # of open slot spots (placeable slots) */
	char column_terminators[AXIS];
};

struct board make_board(void)
{
	struct board b;
	enum edge edges[5] = { NONE, NONE, NONE, NONE, NONE };
	const unsigned int mid = (AXIS - 1) / 2; /* Must start in center. */
	b.slot_spots[0] = make_slot(mid, mid);
	b.sps = 1;
	for (unsigned int i = 0; i < AXIS*AXIS; ++i) {
		b.tiles[i] = create_tile(edges);
	}
	/* Tab between columns except for the last one, which newlines. */
	memset(b.column_terminators, '\t', AXIS - 1);
	b.column_terminators[AXIS - 1] = '\n';
	return b;
}

=======
>>>>>>> origin/dev
=======
>>>>>>> Nick495/dev
static size_t index_slot(struct slot s)
{
	return AXIS * s.x + s.y;
}

<<<<<<< HEAD
<<<<<<< HEAD
void print_board(struct board b)
{
	const size_t cnt = TILE_LINES;
	const size_t len = TILE_LINE_LEN;
	char res[(AXIS * AXIS) * (TILE_LEN - 1) + 1]; /* Null terminators */
	char buf[TILE_LEN];

	/* Pretty print the board in NxN format. */
	for (size_t i = 0; i < AXIS; ++i) {
		for (size_t j = 0; j < AXIS; ++j) {
			print_tile(b.tiles[index_slot(make_slot(i, j))], buf);
			for (size_t k = 0; k < cnt; ++k) {
				const size_t ind = ((i *cnt +k) *AXIS +j) *len;
				buf[(k + 1) *len - 1] = b.column_terminators[j];
				memcpy(&res[ind], &buf[len * k], len);
			}
		}
	}
	res[(AXIS * AXIS) * (TILE_LEN - 1)] = '\0';
	printf("%s\n\n", res);
}

=======
>>>>>>> origin/dev
=======
>>>>>>> Nick495/dev
static int slot_placeable(struct board b, struct slot s)
{
	/* TODO: Switch to linear search? */
	/* Linear search open positions for the desired one. */
	for (unsigned i = 0; i < b.sps; ++i) {
		switch(compare_slots(b.slot_spots[i], s)) {
		case -1:
			continue;
		case 0:
			return i + 1;
		case 1:
			return 0;
		}
	}
	return 0;
}

static int slot_empty(struct board b, struct slot s)
{
	struct tile t = b.tiles[index_slot(s)];
	for (int i = 0; i < 5; ++i) {
		if (t.edges[i] != EMPTY) {
			return 0;
		}
	}
	return 1;
}

static int slot_on_board(struct slot s)
{
	if (s.x < AXIS && s.y < AXIS) {
		return 1;
	}
	return 0;
}

size_t find_spot(struct slot *slots, size_t count, struct slot s)
{
	size_t i;
	for (i = 0; i < count && compare_slots(s, slots[i]) > 0; ++i) {}
	return i;
}

static struct board add_placeable_slot(struct board b, struct slot s)
{
	struct slot *spots = b.slot_spots;
	size_t i = find_spot(spots, b.sps, s);
	if (i < b.sps) { /* Make room for the element (Sorted insert). */
		memmove(&b.slot_spots[i + 1], &spots[i], sizeof(s) * b.sps - i);
	}
	spots[i] = s;
	b.sps++;
	return b;
}

static struct board remove_placeable_slot(struct board b, struct slot s)
{
	struct slot *spots = b.slot_spots;
	size_t i = find_spot(spots, b.sps, s);
	memmove(&spots[i], &spots[i + 1], sizeof(s) * (b.sps-- - i));
	return b;
}

static struct board update_slot_spots(struct board b, struct slot s)
{
	/* Check the slots above, left, right, and below. */
	struct slot adj[4] = { 
		make_slot(s.x, s.y - 1), make_slot(s.x - 1, s.y),
		make_slot(s.x + 1, s.y), make_slot(s.x, s.y + 1)
	};
	b = remove_placeable_slot(b, s);
	for (int i = 0; i < 4; ++i) {
		if (slot_empty(b, adj[i]) && slot_on_board(adj[i])) {
			b = add_placeable_slot(b, adj[i]);
		}
	}
	return b;
}

<<<<<<< HEAD
<<<<<<< HEAD
struct move {
	struct tile tile;
	struct slot slot;
	int rotation;
};

struct move make_move(struct tile t, struct slot s, int rotation)
{
	struct move m = {
		.tile = t,
		.slot = s,
		.rotation = rotation
	};
	return m;
}

static int validate_move(struct board b, struct move m)
=======
/* TODO: Switch int error codes to error enums for cleanliness. */
static int invalid_move(struct board b, struct move m)
>>>>>>> origin/dev
=======
/* TODO: Switch int error codes to error enums for cleanliness. */
static int invalid_move(struct board b, struct move m)
>>>>>>> Nick495/dev
{
	if (!slot_placeable(b, m.slot)) {
		return 1; /* Slot not placeable. */
	}
	/* Check adjacent tiles to make sure edges match. */
	struct slot adj[4] = {
		make_slot(m.slot.x, m.slot.y + 1),	/* up */
		make_slot(m.slot.x + 1, m.slot.y),	/* right */
		make_slot(m.slot.x, m.slot.y - 1),	/* down */
		make_slot(m.slot.x - 1, m.slot.y)	/* left*/
	};
	for (unsigned int i = 0; i < sizeof(adj); ++i) { /* Need wrapping */
		if (!slot_on_board(adj[i])) { /* ignore if not on board. */
			continue;
		}
		/* The (i + 2) % 4 math here is a bit evil, but it works. */
		enum edge pair = b.tiles[index_slot(adj[i])].edges[(i + 2) % 4];
<<<<<<< HEAD
<<<<<<< HEAD
		if (pair == NONE) {
=======
		if (pair == EMPTY) {
>>>>>>> origin/dev
=======
		if (pair == EMPTY) {
>>>>>>> Nick495/dev
			continue; /* Empty tiles match with everything. */
		}
		if (pair != m.tile.edges[i]) { /* Corresponding don't match. */
			return 2;
<<<<<<< HEAD
=======
		}
	}
	return 0;
}

struct board make_board(void)
{
       struct board b;
       enum edge edges[5] = { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
       const unsigned int mid = (AXIS - 1) / 2; /* Must start in center. */
       b.slot_spots[0] = make_slot(mid, mid);
       b.sps = 1;
       for (unsigned int i = 0; i < AXIS*AXIS; ++i) {
		b.tiles[i] = make_tile(edges, NONE);
       }
       /* Tab between columns except for the last one, which newlines. */
       memset(b.column_terminators, '\t', AXIS - 1);
       b.column_terminators[AXIS - 1] = '\n';
       return b;
}

char *print_board(struct board b, char res[BOARD_LEN])
{
	const size_t cnt = TILE_LINES;
	const size_t len = TILE_LINE_LEN;
	char buf[TILE_LEN];

	/* Pretty print the board in NxN format. */
	for (size_t i = 0; i < AXIS; ++i) {
		for (size_t j = 0; j < AXIS; ++j) {
			print_tile(b.tiles[index_slot(make_slot(i, j))], buf);
			for (size_t k = 0; k < cnt; ++k) {
				const size_t ind = ((i *cnt +k) *AXIS +j) *len;
				buf[(k + 1) *len - 1] = b.column_terminators[j];
				memcpy(&res[ind], &buf[len * k], len);
			}
>>>>>>> Nick495/dev
		}
	}
	res[BOARD_LEN - 1] = '\0';
	return res;
}

/* See TODO for invalid_move. */
int play_move_board(struct board *b, struct move m)
{
	int rc;
	if ((rc = invalid_move(*b, m))) {
		return rc;
	}
	b->tiles[index_slot(m.slot)] = rotate_tile(m.tile, m.rotation);
	*b = update_slot_spots(*b, m.slot);
	return 0;
}

#ifdef TEST
static void print_placeable_slots(struct board b)
<<<<<<< HEAD
{
	printf("Slots:\n");
	printf("X\tY\n");
	for (size_t i = 0; i < b.sps; ++i) {
		printf("%u\t%u\n", b.slot_spots[i].x, b.slot_spots[i].y);
	}
	return;
}

static void play_and_check_move(struct board *b, struct move m)
{
=======
{
	printf("Slots:\n");
	printf("X\tY\n");
	for (size_t i = 0; i < b.sps; ++i) {
		printf("%u\t%u\n", b.slot_spots[i].x, b.slot_spots[i].y);
	}
	return;
}

static void play_and_check_move(struct board *b, struct move m)
{
>>>>>>> Nick495/dev
	int rc;
	if ((rc = play_move_board(b, m))) {
		printf("Invalid move! %d\n", rc);
	} else {
		printf("Good move!\n");
	}
}

int main(void)
{
	char buffer[TILE_LEN];
<<<<<<< HEAD
<<<<<<< HEAD
=======
	char board_buffer[BOARD_LEN];
>>>>>>> origin/dev
	enum edge edges[5][5] = {
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ GAME-TRAIL, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL, GAME-TRAIL },
		{ JUNGLE, JUNGLE, JUNGLE, JUNGLE, JUNGLE },
		{ LAKE, LAKE, LAKE, LAKE, LAKE },
		{ LAKE, JUNGLE, GAME-TRAIL, LAKE, GAME-TRAIL }
=======
	char board_buffer[BOARD_LEN];
	enum edge edges[5][5] = {
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ ROAD, ROAD, ROAD, ROAD, ROAD },
		{ FIELD, FIELD, FIELD, FIELD, FIELD },
		{ CITY, CITY, CITY, CITY, CITY },
		{ CITY, FIELD, ROAD, CITY, ROAD }
>>>>>>> Nick495/dev
	};
	struct tile tiles[5] = {
		make_tile(edges[0], NONE),
		make_tile(edges[1], NONE),
		make_tile(edges[2], NONE),
		make_tile(edges[3], NONE),
		make_tile(edges[4], NONE)
	};

	const char string[5][30] = {
		"\nEmpty tile:",
<<<<<<< HEAD
		"\nAll GAME-TRAIL tile:",
		"\nAll JUNGLE tile:",
		"\nAll LAKE tile:",
=======
		"\nAll Road tile:",
		"\nAll Field tile:",
		"\nAll City tile:",
>>>>>>> Nick495/dev
		"\nMixed tile:"
	};

	printf("Testing different tile types.\n");
	for (int i = 0; i < 5; ++i) {
		printf("%s\n%s\n", string[i], print_tile(tiles[i], buffer));
	}

	printf("\nTile Rotations: \n");
	for (int i = 0; i < 4; ++i) {
		printf("%d rotation:\n%s\n", i,
			print_tile(rotate_tile(tiles[4], i), buffer));
	}

	printf("\nTesting board creation. All Null.\n");
	struct board b = make_board();
	printf("%s\n", print_board(b, board_buffer));

	printf("\nLet's see what slots are placeable.\n");
	print_placeable_slots(b);
	const unsigned int mid = AXIS / 2;
	printf("\nPlay the center (%d, %d), the starting move.\n", mid, mid);
	play_and_check_move(&b, make_move(tiles[3], make_slot(mid, mid), 0));
	printf("%s\n", print_board(b, board_buffer));

	printf("\nAnd now what slots are placeable?\n");
	print_placeable_slots(b);

	printf("\nTest tile validator (should fail): (%d, %d)\n", mid, mid + 1);
	play_and_check_move(&b,make_move(tiles[2], make_slot(mid, mid + 1), 0));
	printf("%s\n", print_board(b, board_buffer));
	print_placeable_slots(b);

	printf("\nTest tile validator (should pass): (%d, %d)\n", mid, mid + 1);
	play_and_check_move(&b,make_move(tiles[3], make_slot(mid, mid + 1), 0));
	printf("%s\n", print_board(b, board_buffer));
	print_placeable_slots(b);

	return 0;
}
#endif
<<<<<<< HEAD

struct board make_board(void)
{
       struct board b;
       enum edge edges[5] = { EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
       const unsigned int mid = (AXIS - 1) / 2; /* Must start in center. */
       b.slot_spots[0] = make_slot(mid, mid);
       b.sps = 1;
       for (unsigned int i = 0; i < AXIS*AXIS; ++i) {
		b.tiles[i] = make_tile(edges, NONE);
       }
       /* Tab between columns except for the last one, which newlines. */
       memset(b.column_terminators, '\t', AXIS - 1);
       b.column_terminators[AXIS - 1] = '\n';
       return b;
}

char *print_board(struct board b, char res[BOARD_LEN])
{
	const size_t cnt = TILE_LINES;
	const size_t len = TILE_LINE_LEN;
	char buf[TILE_LEN];

	/* Pretty print the board in NxN format. */
	for (size_t i = 0; i < AXIS; ++i) {
		for (size_t j = 0; j < AXIS; ++j) {
			print_tile(b.tiles[index_slot(make_slot(i, j))], buf);
			for (size_t k = 0; k < cnt; ++k) {
				const size_t ind = ((i *cnt +k) *AXIS +j) *len;
				buf[(k + 1) *len - 1] = b.column_terminators[j];
				memcpy(&res[ind], &buf[len * k], len);
			}
		}
	}
	res[BOARD_LEN - 1] = '\0';
	return res;
}

/* See TODO for invalid_move. */
int play_move_board(struct board *b, struct move m)
{
	int rc;
	if ((rc = invalid_move(*b, m))) {
		return rc;
	}
	b->tiles[index_slot(m.slot)] = rotate_tile(m.tile, m.rotation);
	*b = update_slot_spots(*b, m.slot);
	return 0;
}
=======
>>>>>>> Nick495/dev
