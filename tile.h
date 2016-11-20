#ifndef TILE_H_
#define TILE_H_

#include <stdio.h>
#include <stdlib.h>	/* malloc() */
#include <string.h>	/* memcpy() */
#include "edge.h"	/* edges. */

#define TILE_LINE_LEN 4
#define TILE_LINES 3
#define TILE_LEN TILE_LINE_LEN * TILE_LINES + 1 /* Null terminator */

<<<<<<< HEAD
<<<<<<< HEAD
=======
enum attribute {
	NONE = 0,
	SHIELD = 1,
	DEN = 2
};

>>>>>>> origin/dev
=======
enum attribute {
	NONE = 0,
	SHIELD = 1,
	MONASTERY = 2
};

>>>>>>> Nick495/dev
struct tile {
	enum edge edges[5]; /* Top, Right, Bottom, Left, Center. */
	enum attribute attribute;
};

<<<<<<< HEAD
=======
int tile_eq(struct tile a, struct tile b);
>>>>>>> Nick495/dev
struct tile make_tile(const enum edge edges[5], enum attribute a);
struct tile rotate_tile(const struct tile old, const int rotation);
char *print_tile(struct tile t, char b[TILE_LEN]);

#endif
