#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "GeneralFunctions.h"
#include "LevelClass.h"

enum class ViewportMode {
	Editor,
	Gameplay
};

class Viewport {
private:
	Viewport(float width, float height);
	RenderTexture2D renderTexture;
	Vector2 virtualSize;
	Vector2 offsets;
	int gridSize = 32;
	ViewportMode setting = ViewportMode::Editor;

	Camera2D camera; // Camera for viewport

	//calculated each frame
	Vector2 renderSize;
	Vector2 renderOffset;
	float renderScale;

public:
	static unique_ptr<Viewport> Create(float width, float height) {
		return unique_ptr<Viewport>(new Viewport(width, height));
	}
	~Viewport();
	void Begin(); // Begin drawing to the viewport
	void End(); // End drawing to the viewport and present it on screen
	void UpdateCameraEditor(float dt); // Update the camera (if needed)
	void UpdateCameraGameplay(float dt, Vector2 target); // Update the camera for gameplay mode
	void Draw(); // Draw the viewport texture to the screen
	void DrawGrid(); // Draw a grid within the viewport
	void DrawBounds(const Level& level); // Draw level bounds for reference
	void DrawBoundsBackground(const Level& level); // Draw a background for the level bounds
	void DrawLevel(Level& level); // Draw the given level within the viewport
	void DrawTile(Tile* tile, Vector2 position); // Draw a single tile at the specified position
	Vector2 ScreenToVirtual(Vector2 mousePos) const; // Convert screen coordinates to virtual viewport coordinates
	Vector2 ScreenToWorld(Vector2 mousePos) const; // Convert screen coordinates to world coordinates
	GridPos WorldToGrid(Vector2 worldPos) const; // Convert world coordinates to grid position

	//getters
	Vector2 GetRenderSize() const { return renderSize; }
	Camera2D& GetCamera() { return camera; };
	int GetGridSize() const { return gridSize; }

	//setters
	void SetViewportMode(ViewportMode mode) { setting = mode; };
	void SetCameraOffset(Vector2 offset) { camera.offset = offset; };
	void SetCameraZoom(float zoom) { camera.zoom = zoom; };

};


#endif