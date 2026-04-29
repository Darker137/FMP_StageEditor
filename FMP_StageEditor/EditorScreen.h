#ifndef EDITORSCREEN_H
#define EDITORSCREEN_H

#include "GeneralFunctions.h"
#include "Screen.h"
#include "TileClass.h"
#include "EditorTopBar.h"

struct Tile;
class MenuBarButton;

struct Selection {
	bool active = false;
	bool isMoving = false;
	GridPos start;
	GridPos end;

	//vector for storing selected tiles if needed in future, currently selection is just visual
	vector<Tile> selectedTiles;
};

class EditorScreen : public Screen {
private:

	//Variables for dragging tiles
	bool isDraggingTile = false;
	bool isTileNew = false;
	Tile draggedTile;
	GridPos dragStartPos;
	Vector2 dragWorldPos;
	
	Vector2 mouseOffset; //offset of mouse from top left corner while dragging, so that selected tile or tiles don't snap to top left corner of mouse position while dragging

	Selection selection;

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
	void UpdateSelection();
	void UpdateMovePlayer();
	void DrawTileDragging();
	void DrawSelection();
	void DrawPlayer();
	void DrawUI();
	void DrawMovingSelection();
	void DrawSelectedTiles();
	
	Rectangle GetSelectionBounds() const;
	void SetSelectionTiles();
	void MoveSelectedTiles();
	void RemoveSelectedTiles();
	void DeleteSelectedTiles();
	void PlaceSelectedTiles(GridPos gridStart, GridPos gridEnd);

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
	void AddToUndoStack();
	void Undo();
	void Redo();
	void KeyShortcuts();

	void NewLevel() { appContext.currentLevel = Level::GetLevel(0); Init(); }
	void SaveLevelAs();
	void SaveOpenLevel();
	void SaveLevelToFile(const string& filePath);
	void LoadLevel();
};

#endif