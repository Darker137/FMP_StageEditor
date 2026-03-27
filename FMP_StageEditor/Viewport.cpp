#include "Viewport.h"

Viewport::Viewport(float width, float height) {
	renderTexture = LoadRenderTexture(width, height);
	virtualSize = { width, height };

	//initial values
	renderSize = { 0,0 };
	renderOffset = { 0,0 };
	renderScale = 1.0f;
	offsets = { 100,100 };

	//setup camera
	camera.target = { 0.0f, 0.0f };    // world position
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
}

Viewport::~Viewport() {
	UnloadRenderTexture(renderTexture);
}

void Viewport::Begin() {
	if (setting == ViewportMode::Editor) {
		// In editor mode, we might want to draw additional UI elements later
		BeginTextureMode(renderTexture);
	}
	BeginMode2D(camera);
}

void Viewport::End() {
	EndMode2D();
	if (setting == ViewportMode::Editor) {
		EndTextureMode();

		Vector2 windowSize = {
			(float)GetScreenWidth() - offsets.x,
			(float)GetScreenHeight() - offsets.y
		};

		float scaleX = (float)windowSize.x / virtualSize.x;
		float scaleY = (float)windowSize.y / virtualSize.y;

		renderScale = fmin(
			scaleX,
			scaleY
		);

		renderSize.x = (int)(virtualSize.x * renderScale);
		renderSize.y = (int)(virtualSize.y * renderScale);

		renderOffset.x = (float)GetScreenWidth() - renderSize.x;
		renderOffset.y = (float)GetScreenHeight() - renderSize.y;

	}
}

void Viewport::UpdateCameraEditor(float dt) {
	// Update camera logic if needed
	camera.offset = {
		virtualSize.x * 0.5f,
		virtualSize.y * 0.5f
	};

	//panning with middle mouse button
	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
		Vector2 mouseDelta = GetMouseDelta();

		//convert from window pizels to viewport virtual pixels
		mouseDelta.x *= virtualSize.x / renderSize.x;
		mouseDelta.y *= virtualSize.y / renderSize.y;

		mouseDelta = Vector2Scale(mouseDelta, -1.0f / camera.zoom);
		camera.target = Vector2Add(camera.target, mouseDelta);
	}

	//Zooming with mouse wheel
	float wheelMove = GetMouseWheelMove();
	if (wheelMove != 0.0f) {
		camera.zoom *= (wheelMove > 0 ? 1.1f : 0.9f);
		camera.zoom = Clamp(camera.zoom, 0.1f, 10.0f);
	}
}

void Viewport::UpdateCameraGameplay(float dt, Vector2 target) {
	// Update camera logic for gameplay mode if needed
	camera.target = target;
}

void Viewport::Draw() {
	// draw scaled texture to screen

	Rectangle src = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
	Rectangle dst = { (float)renderOffset.x, (float)renderOffset.y, (float)renderSize.x, (float)renderSize.y };
	DrawTexturePro(renderTexture.texture, src, dst, { 0,0 }, 0.0f, WHITE);
}

void Viewport::DrawGrid() {
	// Draw grid lines within the viewport
	Color minor = { 100, 100, 100, 20 };
	Color major = { 150, 150, 150, 20 };

	float left = camera.target.x - (virtualSize.x * 0.5f) / camera.zoom;
	float top = camera.target.y - (virtualSize.y * 0.5f) / camera.zoom;
	float right = camera.target.x + (virtualSize.x * 0.5f) / camera.zoom;
	float bottom = camera.target.y + (virtualSize.y * 0.5f) / camera.zoom;

	float startX = floor(left / gridSize) * gridSize;
	float startY = floor(top / gridSize) * gridSize;

	for (float x = startX; x <= right; x += gridSize) {
		DrawLine(x, top , x, bottom, minor);
	}

	for (float y = startY; y <= bottom; y += gridSize) {
		DrawLine(left, y, right, y, minor);
	}
}

// draw outline around the level bounds for reference
void Viewport::DrawBounds(const Level& level) {
	// Draw level bounds for reference
	float width = level.GetWidth() * gridSize;
	float height = level.GetHeight() * gridSize;
	//DrawRectangleLines(0, 0, width, height, RAYWHITE);
/*
	//top line
	DrawLineEx({ -2,-1 }, { width + 2 , -1 }, 2, RAYWHITE);
	//bottom line
	DrawLineEx({ -2, height + 1 }, { width + 2 , height + 1 }, 2, RAYWHITE);
	//left line
	DrawLineEx({ -1, -2 }, { -1 , height + 2 }, 2, RAYWHITE);
	//right line
	DrawLineEx({ width + 1, -2 }, { width + 1 , height + 2 }, 2, RAYWHITE);
*/
	//regular draw line version
	//Top line
	DrawLine(0, 0, width, 0, RAYWHITE);
	//Bottom line
	DrawLine(0, height, width, height, RAYWHITE);
	//Left line
	DrawLine(0, 0, 0, height, RAYWHITE);
	//Right line
	DrawLine(width, 0, width, height, RAYWHITE);


}

void Viewport::DrawBoundsBackground(const Level& level) {
	// Draw a background for the level bounds
	float width = level.GetWidth() * gridSize;
	float height = level.GetHeight() * gridSize;
	DrawRectangle(0, 0, width, height, { 50, 50, 50, 255 });
}

void Viewport::DrawLevel(Level& level) {
	// Draw the given level within the viewportS
	for (int y = 0; y < level.GetHeight(); ++y) {
		for (int x = 0; x < level.GetWidth(); x++) {
			if (level.TileExists(x,y)) {
				Vector2 pos = {
					(float)(x * gridSize),
					(float)(y * gridSize)
				};
				Tile* tile = level.GetTile(x, y);
				DrawTile(level.GetTile(x,y), pos);
			}
		}
	}
}

void Viewport::DrawTile(Tile* tile, Vector2 position) {
	// Draw tile rectangle
	Color color = tile->color;
	//print colour value to console
	cout << "Drawing tile at (" << position.x << ", " << position.y << ") with color ("
		<< (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")" << endl;
	DrawRectangle(
		position.x,
		position.y,
		(float)gridSize,
		(float)gridSize,
		color
	);
}

Vector2 Viewport::ScreenToVirtual(Vector2 mousePos) const {
	return {
		(mousePos.x - renderOffset.x) / renderScale,
		(mousePos.y - renderOffset.y) / renderScale
	};
}

Vector2 Viewport::ScreenToWorld(Vector2 mousePos) const {
	Vector2 virtualPos = ScreenToVirtual(mousePos);
	return GetScreenToWorld2D(virtualPos, camera);
}

GridPos Viewport::WorldToGrid(Vector2 worldPos) const {
	return {
		(int)(worldPos.x) / gridSize,
		(int)(worldPos.y) / gridSize
	};
}