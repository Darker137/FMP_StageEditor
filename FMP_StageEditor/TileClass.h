#ifndef TILECLASS_H
#define TILECLASS_H

#include "GeneralFunctions.h"

enum class TileState 
{
	//add more states later
	NORMAL
};

struct Tile
{
	int id = 0;
	TileState state = TileState::NORMAL;
};

struct TileData
{
	int id = 0;
	int textureID = 0;
	Color color = WHITE;
};

#endif