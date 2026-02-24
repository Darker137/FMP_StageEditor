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
	vector<Tile> tiles; // Collection of tiles in the level

	Level(int id, int tileSize, int width, int height);
public:
	static unique_ptr<Level> GetLevel(int id) {
		return unique_ptr<Level>(new Level(id, 32, 10, 10));
	}

	~Level() {};

	void PlayerTileCollision(PlayerData& data);

	// Accessor for tiles
	Tile& GetTile(int x, int y) {
		return tiles[y * width + x];
	}
	const Tile& GetTile(int x, int y) const {
		return tiles[y * width + x];
	}

	void SetTile(int x, int y, const Tile& tile) {
		tiles[y * width + x] = tile;
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