#include "levelClass.h"

Level::Level(int id, int tileSize, int width, int height)
	: id(id), tileSize(tileSize), width(width), height(height) {
	tiles.resize(width * height);
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
			Tile& tile = GetTile(x, y);
			//Tile world bound
			Rectangle tileRect = {
				x * tileSize,
				y * tileSize,
				(float)tileSize,
				(float)tileSize
			};
			if (CheckCollisionRecs(playerRect, tileRect) && tile.collision) {
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