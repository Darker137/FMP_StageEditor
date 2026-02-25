#ifndef LEVELCLASS_H
#define LEVELCLASS_H

#include "GeneralFunctions.h"
#include "TileClass.h"
#include "Player.h"

class Level {
private:
	int id; // Level identifier
	int tileSize; // Size of each tile in pixels
	int width, height; // Dimensions of the level in tiles
	vector<Tile*> tiles; // Collection of tiles in the level

	Level(int id, int tileSize, int width, int height);
public:
	static unique_ptr<Level> GetLevel(int id) {
		return unique_ptr<Level>(new Level(id, 32, 50, 30));
	}

	~Level() {};

	void PlayerTileCollision(PlayerData& data);

	// Accessor for tiles
	/*
	Tile& GetTile(int x, int y) {
		return *tiles[y * width + x];
	}
	*/
	//get tile pointer

	//check tile existence
	bool TileExists(int x, int y) const {
		return tiles[y * width + x] != nullptr;
	}

	Tile* GetTile(int x, int y) 
	{
	
		return tiles[y * width + x];
	}

	void SetTile(int x, int y, Tile* tile) {
		//replace tile info with new tile info
		tiles[y * width + x] = tile;
	}

	void DeleteTile(int x, int y) {
		if (tiles[y * width + x] != nullptr) {
			delete tiles[y * width + x];
			tiles[y * width + x] = nullptr;
		}
	}
	void ReplaceTileWithPointer(int x, int y, Tile* tile) {
		//delete old tile if it exists
		DeleteTile(x, y);
		//set new tile
		tiles[y * width + x] = tile;
	}
	void ReplaceTileWithValue(int x, int y, Tile tile) {
		//delete old tile if it exists
		DeleteTile(x, y);
		//create new tile and set it
		tiles[y * width + x] = new Tile(tile);
	}

	bool InBounds(int x, int y) const {
		return (x >= 0 && x < width && y >= 0 && y < height);
	}
	bool PlayerInBounds(PlayerData& data) const {
		return (data.position.x >= 0 && (data.position.x + data.size.x) <= (width * tileSize) &&
				data.position.y >= 0 && (data.position.y + data.size.y) <= (height * tileSize));
	}

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetTileSize() const { return tileSize; }
};

#endif