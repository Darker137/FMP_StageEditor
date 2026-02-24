#ifndef PLAYER_H
#define PLAYER_H

#include "GeneralFunctions.h"

struct PlayerData {
	Vector2 position = { 0,0 };
	Vector2 size = { 16,16 };
	Color colour = BLUE;
};

class Player {
private:
	
	PlayerData data;
	Vector2 Velocity;
	float speed;

	Player(PlayerData& data) 
		: data(data),
		  Velocity({ 0.0f, 0.0f }),
		  speed(200.0f) {}
public:
	static unique_ptr<Player> Create(PlayerData& data) {
		return unique_ptr<Player>(new Player(data));
	}
	void Update(float dt);
	void Draw() const;

	//getters
	// Return a reference to the player's data
	PlayerData& GetData() { return data; }
};

#endif