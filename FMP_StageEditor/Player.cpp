#include "player.h"

void Player::Update(float dt) {
	Velocity = { 0.0f, 0.0f };
	if (IsKeyDown(KEY_W)) {
		Velocity.y -= 1.0f;
	}
	if (IsKeyDown(KEY_S)) {
		Velocity.y += 1.0f;
	}
	if (IsKeyDown(KEY_A)) {
		Velocity.x -= 1.0f;
	}
	if (IsKeyDown(KEY_D)) {
		Velocity.x += 1.0f;
	}
	// Normalize velocity to prevent faster diagonal movement
	if (Vector2Length(Velocity) > 0.0f) {
		Velocity = Vector2Normalize(Velocity);
	}
	// Update position
	data.position = Vector2Add(data.position, Vector2Scale(Velocity, speed * dt));
}

void Player::Draw() const {
	DrawRectangleV(data.position, data.size, data.colour);
}