#ifndef LEVELCLASS_H
#define LEVELCLASS_H

#include "GeneralFunctions.h"
#include "TileClass.h"

struct Tile;
struct TileData;
struct PlayerData;

class Level {
private:
	int id; // Level identifier
	int tileSize; // Size of each tile in pixels
	int width, height; // Dimensions of the level in tiles
	vector<Tile>tileList; //will replace tiles vector below to save on unnecessary storage of multiple of the same tile type.
	vector<TileData> existingTiles; // Collection of tiles in the level, pointer as to reference the same tile data in screen class for use in tile selection and reference when adding tiles in editor
	unordered_set<int> usedTileIDs; // Set of unique tile IDs used in the level
	Vector2 playerStartPos; // Starting position of the player in the level

	string levelPath;

	Level(int id, int tileSize, int width, int height);
public:
	static unique_ptr<Level> GetLevel(int id) {
		return unique_ptr<Level>(new Level(id, 32, 50, 30));
	}

	~Level() {};

	void PlayerTileCollision(PlayerData& data);

	void DeleteTile(int x, int y);

	void ReplaceTileWithPointer(int x, int y, Tile* tile);

	void ReplaceTileWithValue(int x, int y, Tile tile);

	void RebuildUsedTileIDs();

	void SetTileData(vector<TileData>* tiles) { existingTiles = *tiles; }

	bool TileExists(int x, int y) const;
	bool InBounds(int x, int y) const { return (x >= 0 && x < width && y >= 0 && y < height); }
	bool PlayerInBounds(PlayerData& data) const;
	bool IsTileIDUsed(int id) const { return usedTileIDs.find(id) != usedTileIDs.end(); }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetTileSize() const { return tileSize; }
	Tile* GetTile(int x, int y) { return  &tileList[y * width + x]; }
	int GetTextureID(int id);
	Color GetTileColor(int id);
	string& GetLevelPath() { return levelPath; }
	Vector2& GetPlayerStartPos() { return playerStartPos; }

	//replace tile info with new tile info
	void SetTile(int x, int y, Tile* tile);
	void SetLevelPath(string path) { levelPath = path; }
	void SetPlayerStartPos(Vector2 pos) { playerStartPos = pos; }
	
	void AddToExistingTiles(const TileData& tileData) { existingTiles.push_back(tileData); }

	void SaveToFile(const string& filePath);

	void LoadFromFile(const string& filePath);
};

#endif