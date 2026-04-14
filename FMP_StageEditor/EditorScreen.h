#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "GeneralFunctions.h"
#include "Screen.h"
#include "TileClass.h"
#include "EditorTopBar.h"

struct Tile;
class MenuBarButton;

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

	unique_ptr<TopBar> topBar = nullptr;

	unique_ptr<DropdownList> tileSelector = nullptr;
	unique_ptr<Button> addButton = nullptr;
	unique_ptr<Button> playButton = nullptr;

	//temporary tile list for selection
	vector<TileData> tileList;

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
	void Init() override;
	void LoadTileTextures() override;
	void LoadTileData() override;

	void NewLevel() { appContext.currentLevel = Level::GetLevel(0); Init(); }
	void SaveLevelAs();
	void SaveOpenLevel();
	void SaveLevelToFile(const string& filePath);
	void LoadLevel();
};

#endif