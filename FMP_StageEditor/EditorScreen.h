#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "GeneralFunctions.h"
#include "Screen.h"

class EditorScreen : public Screen {
private:

	//Variables for dragging tiles
	bool isDraggingTile = false;
	bool isTileNew = false;
	Tile draggedTile;
	GridPos dragStartPos;
	Vector2 dragWorldPos;

	//moving player
	bool isMovingPlayer = false;
	Vector2 playerStartPos;

	unique_ptr<DropdownList> tileSelector = nullptr;
	unique_ptr<Button> addButton = nullptr;
	unique_ptr<Button> playButton = nullptr;

	//temporary tile list for selection
	vector<Tile> tileList = {
	Tile{}, // Empty Tile
	{ 1, true, WHITE  }, // Tile 1
	{ 2, true, RED }, // Tile 2
	{ 3, true, GREEN }  // Tile 3
	};

	void UpdateViewportInput();
	void UpdateTileDragging();
	void UpdateMovePlayer();
	void DrawTileDragging();
	void DrawPlayer();
	void DrawUI();

	void AddTile();
	void PlayButton();

public:
	EditorScreen(AppContext& context);
	~EditorScreen() override;
	void Update() override;
	void Draw() override;
};

#endif