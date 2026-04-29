#include "levelClass.h"
#include "TileClass.h"
#include "Player.h"
#include "TileClass.h"


Level::Level(int id, int tileSize, int width, int height)
	: id(id), tileSize(tileSize), width(width), height(height) {
	tileList.resize(width * height);
	playerStartPos = { 0,0 }; // Default player start position, can be set later
}

void Level::PlayerTileCollision(PlayerData& data) {
	// Implement player-tile collision logic here

	// Determine the player's bounding rectangle
	Rectangle playerRect = {
		data.position.x,
		data.position.y,
		data.size.x,
		data.size.y
	};

	// Calculate the range of tiles the player occupies
	int leftTile = (int)(playerRect.x / tileSize);
	int rightTile = (int)((playerRect.x + playerRect.width) / tileSize);
	int topTile = (int)(playerRect.y / tileSize);
	int bottomTile = (int)((playerRect.y + playerRect.height) / tileSize);

	// Clamp tile indices to level bounds
	leftTile = fmax(0, leftTile);
	rightTile = fmin(width - 1, rightTile);
	topTile = fmax(0, topTile);
	bottomTile = fmin(height - 1, bottomTile);

	for (int y = topTile; y <= bottomTile; y++){
		for (int x = leftTile; x <= rightTile; x++)
		{
			if (!TileExists(x, y)) continue; // Skip empty tiles
			//Tile world bound
			Rectangle tileRect = {
				x * tileSize,
				y * tileSize,
				(float)tileSize,
				(float)tileSize
			};
			if (CheckCollisionRecs(playerRect, tileRect)) {
				// Simple collision response: push player out of tile
				// Calculate overlap on each side
				float overlapLeft = (playerRect.x + playerRect.width) - tileRect.x;
				float overlapRight = (tileRect.x + tileRect.width) - playerRect.x;
				float overlapTop = (playerRect.y + playerRect.height) - tileRect.y;
				float overlapBottom = (tileRect.y + tileRect.height) - playerRect.y;
				// Find the minimum overlap
				float minOverlapX = fmin(overlapLeft, overlapRight);
				float minOverlapY = fmin(overlapTop, overlapBottom);
				// Resolve collision based on the smallest overlap
				if (minOverlapX < minOverlapY) {
					// Resolve in X direction
					if (overlapLeft < overlapRight) {
						data.position.x -= overlapLeft; // Move left
					}
					else {
						data.position.x += overlapRight; // Move right
					}
				}
				else {
					// Resolve in Y direction
					if (overlapTop < overlapBottom) {
						data.position.y -= overlapTop; // Move up
					}
					else {
						data.position.y += overlapBottom; // Move down
					}
				}
			}
		}
	}


}

void Level::DeleteTile(int x, int y) {
	if (tileList[y * width + x].id != 0) {
		tileList[y * width + x] = Tile(); // Reset to default tile
	}
}

void Level::ReplaceTileWithPointer(int x, int y, Tile* tile) {
	//delete old tile if it exists
	DeleteTile(x, y);
	//set new tile
	tileList[y * width + x] = *tile;
}

void Level::ReplaceTileWithValue(int x, int y, Tile tile) {
	//delete old tile if it exists
	DeleteTile(x, y);
	//create new tile and set it
	tileList[y * width + x] = tile;
}

void Level::RebuildUsedTileIDs() {
	usedTileIDs.clear();
	for (const auto& tile : tileList) {
		if (tile.id != 0) {
			//if tile ID is not already in usedTileIDs, add it
			if (usedTileIDs.find(tile.id) == usedTileIDs.end()) {
				usedTileIDs.insert(tile.id);
				cout << "Added tile ID " << tile.id << " to usedTileIDs set." << endl;
			}
			else 
			{
				cout << "Tile ID " << tile.id << " is already in usedTileIDs set." << endl;
			}
		}
	}
}

bool Level::TileExists(int x, int y) const 
{
	return tileList[y * width + x].id != 0; 
}

int Level::GetTextureID(int id) {
	for (const auto& tile : existingTiles) {
		if (tile.id == id) {
			return tile.textureID;
		}
	}
	return -1; // Return -1 if tile ID not found
}

Color Level::GetTileColor(int id) {
	for (const auto& tile : existingTiles) {
		if (tile.id == id) {
			return tile.color;
		}
	}
	return WHITE; // Return default color if tile ID not found
}

bool Level::PlayerInBounds(PlayerData& data) const {
	return (data.position.x >= 0 && (data.position.x + data.size.x) <= (width * tileSize) &&
		data.position.y >= 0 && (data.position.y + data.size.y) <= (height * tileSize));
}

void Level::SetTile(int x, int y, const Tile& tile) { tileList[y * width + x] = tile; }

void Level::SaveToFile(const string& filePath) {
	ofstream file(filePath);

	if (!file.is_open()) {
		cerr << "Failed to open file for saving: " << filePath << endl;
		return;
	}

	file << "# level metadata" << endl;
	file << "id" << id << endl;
	file << "width " << width << endl;
	file << "height " << height << endl;
	file << "# player start position" << endl;
	file << "playerStart " << playerStartPos.x << " " << playerStartPos.y << endl;

	file << "# tile data" << endl;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			const Tile& tile = tileList[y * width + x];
			if (tile.id != 0) { // Only save non-empty tiles
				//save tile data in format: x y tileID tile properties (if any)
				file << x << " " << y << " " << tile.id << " " << static_cast<int>(tile.state) << endl;
			}
		}
	}
}

void Level::LoadFromFile(const string& filePath) {
	ifstream file(filePath);
	if (!file.is_open()) {
		cerr << "Failed to open file for loading: " << filePath << endl;
		return;
	}
	string line;
	while (getline(file, line)) {
		if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
		istringstream iss(line);
		string key;
		iss >> key;
		if (key == "id") {
			iss >> id;
		}
		else if (key == "width") {
			iss >> width;
		}
		else if (key == "height") {
			iss >> height;
			// Resize tileList based on loaded dimensions
			tileList.clear();
			tileList.resize(width * height);
		}
		else if (key == "playerStart") {
			float x, y;
			if (!(iss >> x >> y)) {
				cerr << "Invalid player start position format in file: " << line << endl;
				continue;
			}
			playerStartPos = { x, y };
		}
		else {
			int x;
			try {
				x = stoi(key); // First value is x coordinate
			}
			catch (const invalid_argument&) {
				cerr << "Invalid tile data format in file: " << line << endl;
				continue;
			}
			int y, tileID, tileStateInt;
			if (!(iss >> y >> tileID >> tileStateInt)) {
				cerr << "Invalid tile data format in file: " << line << endl;
				continue;
			}
			if (x >= 0 && x < width && y >= 0 && y < height) {
				tileList[y * width + x] = { tileID, static_cast<TileState>(tileStateInt) };
			}
			else {
				cerr << "Tile position out of bounds in file: " << line << endl;
			}
		}
	}
}