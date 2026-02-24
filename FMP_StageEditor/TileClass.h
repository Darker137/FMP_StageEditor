#ifndef TILECLASS_H
#define TILECLASS_H

#include "GeneralFunctions.h"

struct Tile {
	int id = 0;
	bool collision = false;
	Color color = WHITE;
};

#endif