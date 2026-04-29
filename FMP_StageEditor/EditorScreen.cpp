#include "EditorScreen.h"
#include "TileClass.h"
#include "LevelClass.h"
#include "EditorTopBar.h"

EditorScreen::EditorScreen(AppContext& context)
	: Screen(context) {

	dragStartPos = { 0,0 };
	dragWorldPos = { 0.0f, 0.0f };
	playerStartPos = { 0.0f, 0.0f };

	tileSelector = make_unique<DropdownList>();
	tileSelector->dropDownLabel = "Select Tile";
	tileSelector->bounds = { 10, 50, 150, 30 };

	addButton = make_unique<Button>();
	addButton->label = "Add Tile";
	addButton->bounds = { 170, 50, 100, 30 };

	playButton = make_unique<Button>();
	playButton->label = "Play";
	playButton->bounds = { 280, 50, 100, 30 };

}

EditorScreen::~EditorScreen() {
	//unload textures in tileTextures
	for (auto& pair : tileTextures) {
		cout << "Unloading texture for Tile ID: " << pair.first << endl;
		UnloadTexture(pair.second);
	}
}

void EditorScreen::UpdateViewportInput() {
	// Implement viewport input update logic here
}

void EditorScreen::UpdateTileDragging() {
	//if dragging player or selection , don't allow tile dragging to start
	if (isMovingPlayer || selection.isMoving) return;

	// Implement tile dragging update logic here
	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());
	GridPos grid = appContext.viewport->WorldToGrid(mouseWorld);

	//Start Dragging
	if (!isDraggingTile && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (appContext.currentLevel->InBounds(grid.x, grid.y)) {
			// Add current level state to undo stack before making changes
			AddToUndoStack();
			Tile* tile = appContext.currentLevel->GetTile(grid.x, grid.y);
			if (appContext.currentLevel->TileExists(grid.x, grid.y)) 
			{
				isDraggingTile = true;
				draggedTile = *tile;
				dragStartPos = grid;
				if (!IsKeyDown(KEY_LEFT_CONTROL)) 
				{
					appContext.currentLevel->DeleteTile(grid.x, grid.y); //remove tile from level while dragging
				}
			}
		}
		else cout << "Out of bounds: " << grid.x << ", " << grid.y << endl;
	}


	//While Dragging
	if (isDraggingTile) {
		dragWorldPos = mouseWorld;
	}

	//Drop

	if (isDraggingTile && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		if (appContext.currentLevel->InBounds(grid.x, grid.y)) 
		{
			// move tile to new location 
			appContext.currentLevel->ReplaceTileWithValue(grid.x, grid.y, draggedTile);
		}
		else // If dropped out of bounds, either discard new tile or return to start position
		{
			if (isTileNew) {
				//Discard new tile if dropped out of bounds
				isTileNew = false;
			}
			else
			{
				//Return to start position if dropped out of bounds
				appContext.currentLevel->ReplaceTileWithValue(dragStartPos.x, dragStartPos.y, draggedTile);
			}
		}
		isDraggingTile = false;
	}
}

void EditorScreen::UpdateSelection() {
	// Implement selection update logic here
	if (!isMovingPlayer && !isDraggingTile) {
		Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());
		// Start selection
		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			selection.active = true;
			selection.start = appContext.viewport->WorldToGrid(mouseWorld);
			selection.end = selection.start;
		}
		// Update selection
		if (selection.active && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			selection.end = appContext.viewport->WorldToGrid(mouseWorld);
		}
		// Finalize selection
		if (selection.active && IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
			SetSelectionTiles();
		}
		//if left mouse is clicked outside of selection, deselect
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(mouseWorld, GetSelectionBounds())) {
			selection.active = false;
			selection.isMoving = false;
		}
	}
}

void EditorScreen::UpdateMovePlayer() {
	// Implement player movement update logic here
	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());
	// Check if right mouse button is pressed and mouse is over player
	Rectangle playerRect = {
		appContext.playerData.position.x,
		appContext.playerData.position.y,
		appContext.playerData.size.x,
		appContext.playerData.size.y
	};
	// If right mouse button is pressed on player, start moving player
	if (CheckCollisionPointRec(mouseWorld, playerRect) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		// If not already moving player, store starting position and save snapshot for undo
		if (!isMovingPlayer)
		{
			playerStartPos = appContext.playerData.position;
			AddToUndoStack();
		}
		isMovingPlayer = true;
	}
	// If left mouse button is released, stop moving player and check if in bounds, if not reset to start position
	if (isMovingPlayer && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		// Check if player is in bounds of level, if not reset to start position
		if (!appContext.currentLevel->PlayerInBounds(appContext.playerData)) {
			appContext.playerData.position = playerStartPos;
		}
		//run collision check after moving player to new position to prevent placing player inside tile
		appContext.currentLevel->PlayerTileCollision(appContext.playerData);
		isMovingPlayer = false;
	}
	if (isMovingPlayer) {
		appContext.playerData.position = mouseWorld;
		appContext.currentLevel->SetPlayerStartPos(mouseWorld);
	}
}

void EditorScreen::DrawTileDragging() {
	if (isDraggingTile) {
		Vector2 snapped = {
			floor(dragWorldPos.x / appContext.viewport->GetGridSize()) * appContext.viewport->GetGridSize(),
			floor(dragWorldPos.y / appContext.viewport->GetGridSize()) * appContext.viewport->GetGridSize()
		};
		DrawRectangle(
			(int)snapped.x,
			(int)snapped.y,
			appContext.viewport->GetGridSize(),
			appContext.viewport->GetGridSize(),
			LIGHTGRAY
		);

		Texture2D* texture = nullptr;
		if (tileTextures.find(appContext.currentLevel->GetTextureID(draggedTile.id)) != tileTextures.end()) {
			texture = &tileTextures[appContext.currentLevel->GetTextureID(draggedTile.id)];
		}
		appContext.viewport->DrawTile(&draggedTile, appContext.viewport->ScreenToWorld(GetMousePosition()), texture, LIGHTGRAY);
	}
}

void EditorScreen::DrawSelection() {
	// Implement selection drawing logic here
	if (selection.active) {
		DrawRectangleLinesEx(GetSelectionBounds(), 2, YELLOW);
	}
}

void EditorScreen::DrawPlayer() {
	// Implement player drawing logic here
	DrawRectangleV(
		appContext.playerData.position,
		appContext.playerData.size,
		appContext.playerData.colour
	);
}

void EditorScreen::DrawUI() {
	// Implement UI drawing logic here
	Vector2 viewportSize = appContext.viewport->GetRenderSize();

	Vector2 UIScale = {
		GetScreenWidth() - viewportSize.x,
		GetScreenHeight() - viewportSize.y
	};

	DrawRectangle(0, 0, UIScale.x, GetScreenHeight(), DARKGRAY); // Left UI panel
	DrawRectangle(0, 0, GetScreenWidth(), UIScale.y, DARKGRAY); // Top UI panel
	tileSelector->Draw();
	addButton->Draw();
	playButton->Draw();
	topBar->Draw();
}

void EditorScreen::DrawMovingSelection() {
	// Implement logic to draw moving selection here
	if (selection.isMoving) {
		DrawSelectedTiles();
		//DrawRectangle(
		//	(int)GetSelectionBounds().x,
		//	(int)GetSelectionBounds().y,
		//	(int)GetSelectionBounds().width,
		//	(int)GetSelectionBounds().height,
		//	{255,255,0,100}
		//);
	}
}

void EditorScreen::DrawSelectedTiles() {
	// Implement logic to draw selected tiles in selection.selectedTiles vector
	for(int x = fmin(selection.start.x, selection.end.x); x <= fmax(selection.start.x, selection.end.x); x++) {
		for (int y = fmin(selection.start.y, selection.end.y); y <= fmax(selection.start.y, selection.end.y); y++) {
			Tile tile = selection.selectedTiles[(x - fmin(selection.start.x, selection.end.x)) * (fmax(selection.start.y, selection.end.y) - fmin(selection.start.y, selection.end.y) + 1) + (y - fmin(selection.start.y, selection.end.y))];
			if (tile.id == 0) continue; //skip empty tiles in selection
			Texture2D* texture = nullptr;
			if (tileTextures.find(appContext.currentLevel->GetTextureID(tile.id)) != tileTextures.end()) {
				texture = &tileTextures[appContext.currentLevel->GetTextureID(tile.id)];
			}
			appContext.viewport->DrawTile(
				&tile, 
				{ (float)(x * appContext.viewport->GetGridSize()), (float)(y * appContext.viewport->GetGridSize()) }, 
				texture, {255,255,0,255}
			);
		}
	}
}

Rectangle EditorScreen::GetSelectionBounds() const {
	// Implement logic to calculate selection bounds here
	Vector2 startWorld = {
		selection.start.x * appContext.viewport->GetGridSize(),
		selection.start.y * appContext.viewport->GetGridSize()
	};
	Vector2 endWorld = {
		(selection.end.x + 1) * appContext.viewport->GetGridSize(),
		(selection.end.y + 1) * appContext.viewport->GetGridSize()
	};
	Vector2 topLeft = {
		fmin(startWorld.x, endWorld.x),
		fmin(startWorld.y, endWorld.y)
	};
	Vector2 size = {
		fabs(endWorld.x - startWorld.x),
		fabs(endWorld.y - startWorld.y)
	};
	return { topLeft.x, topLeft.y, size.x, size.y };
}

void EditorScreen::SetSelectionTiles() {
	//logic to set selected tiles in selection.selectedTiles vector
	selection.selectedTiles.clear();
	for (int x = fmin(selection.start.x, selection.end.x); x <= fmax(selection.start.x, selection.end.x); x++) {
		for (int y = fmin(selection.start.y, selection.end.y); y <= fmax(selection.start.y, selection.end.y); y++) {
			selection.selectedTiles.push_back(*appContext.currentLevel->GetTile(x, y));
		}
	}
}

void EditorScreen::MoveSelectedTiles() {
	// move selected tiles in selection.selectedTiles vector
	if (selection.selectedTiles.empty()) return;
	if (isMovingPlayer || isDraggingTile) return;

	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());

	// check if user is trying to move tiles
	if (selection.active && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouseWorld, GetSelectionBounds()))
	{
		AddToUndoStack();
		selection.isMoving = true;
		if (!IsKeyDown(KEY_LEFT_CONTROL)) {
			RemoveSelectedTiles(); //remove tiles from level while moving to prevent duplicates and allow for smoother movement
		}

		//get mouse offset from selection start position in world for smooth dragging without snapping top left corner to mouse position
		mouseOffset = {
			mouseWorld.x - (selection.start.x * appContext.viewport->GetGridSize()),
			mouseWorld.y - (selection.start.y * appContext.viewport->GetGridSize())
		};
	}
	if (selection.isMoving) {
		//calculate mouseOffset based off where the mouse is relative to the selection start position in world coordinates, then apply that offset to the selection start position to get new grid position for each tile in selection and move them to new position
		
		GridPos originalStart = selection.start;
		GridPos originalEnd = selection.end;
		
		Vector2 newStartWorld = {
			mouseWorld.x - mouseOffset.x,
			mouseWorld.y - mouseOffset.y	
		};

		selection.start = appContext.viewport->WorldToGrid(newStartWorld);
		selection.end = {
			selection.start.x + (originalEnd.x - originalStart.x),
			selection.start.y + (originalEnd.y - originalStart.y)
		};

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			//check if new position is in bounds, if not cancel move
			if (!appContext.currentLevel->InBounds(selection.start.x, selection.start.y) ||
				!appContext.currentLevel->InBounds(selection.end.x, selection.end.y)) 
			{
				PlaceSelectedTiles(originalStart, originalEnd);
				selection.isMoving = false;
				return;
			}
			else 
			{
				PlaceSelectedTiles(selection.start, selection.end);
				selection.isMoving = false;
			}
		}

	}
}

void EditorScreen::RemoveSelectedTiles() {
	// remove selected tiles in selection.selectedTiles
	if (selection.selectedTiles.empty()) return;
	if (isMovingPlayer || isDraggingTile) return;
	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());

	//use selected tiles start and end positions to delete tiles in level
	for (int x = fmin(selection.start.x, selection.end.x); x <= fmax(selection.start.x, selection.end.x); x++) {
		for (int y = fmin(selection.start.y, selection.end.y); y <= fmax(selection.start.y, selection.end.y); y++) {
			if (appContext.currentLevel->TileExists(x, y)) {
				appContext.currentLevel->DeleteTile(x, y);
			}
		}
	}
}

void EditorScreen::DeleteSelectedTiles() {
	// essentially the same as RemoveSelectedTiles but also clears selection after deleting tiles and adds to undo stack
	AddToUndoStack();
	RemoveSelectedTiles();
}

void EditorScreen::PlaceSelectedTiles(GridPos gridStart, GridPos gridEnd) {
	// place selected tiles in selection.selectedTiles vector at new position based on gridStart and gridEnd
	if (selection.selectedTiles.empty()) return;
	if (isMovingPlayer || isDraggingTile) return;

	for (int x = fmin(gridStart.x, gridEnd.x); x <= fmax(gridStart.x, gridEnd.x); x++) {
		for (int y = fmin(gridStart.y, gridEnd.y); y <= fmax(gridStart.y, gridEnd.y); y++) {
			Tile tile = selection.selectedTiles[(x - fmin(selection.start.x, selection.end.x)) * (fmax(selection.start.y, selection.end.y) - fmin(selection.start.y, selection.end.y) + 1) + (y - fmin(selection.start.y, selection.end.y))];
			appContext.currentLevel->ReplaceTileWithValue(x, y, tile);
		}
	}
}

void EditorScreen::AddTile() {
	// Implement add tile logic here
	if (addButton->IsClicked() && tileSelector->selectedItem.id != -1) {
		cout << "Adding Tile ID: " << tileSelector->selectedItem.id << endl;
		// Additional logic to add the selected tile can be implemented here
		//find tile in tileList with matching ID from tileSelector and set as draggedTile to be placed in level#
		//draggedTile = tileList[tileSelector->selectedItem.id];
		for (const TileData& tile : tileList) {
			if (tile.id == tileSelector->selectedItem.id) {
				AddToUndoStack();
				Tile newTile = { tile.id, TileState::NORMAL };
				draggedTile = newTile;
				break;
			}
		}
		isDraggingTile = true;
		isTileNew = true;
	}
}

void EditorScreen::PlayButton() {
	// Implement play button logic here
	if (playButton->IsClicked()) {
		cout << "Switching to Gameplay Screen" << endl;
		// Additional logic to switch to gameplay screen can be implemented here
		switchScreen = true;
	}
}

void EditorScreen::Update() {
	Vector2 mousePos = GetMousePosition();
	// Implement editor screen update logic here
	appContext.viewport->UpdateCameraEditor(GetFrameTime());
	UpdateMovePlayer();
	MoveSelectedTiles();
	UpdateTileDragging();
	UpdateSelection();
	tileSelector->Update();
	addButton->Update(mousePos);
	playButton->Update(mousePos);
	topBar->Update(mousePos);
	AddTile();
	PlayButton();
	KeyShortcuts();
}

void EditorScreen::Draw() {

	// Viewport drawing
	appContext.viewport->Begin();

	ClearBackground({ 10,10,10 ,225 });

	appContext.viewport->DrawBoundsBackground(*appContext.currentLevel);
	appContext.viewport->DrawGrid();
	appContext.viewport->DrawBounds(*appContext.currentLevel);
	appContext.viewport->DrawLevel(*appContext.currentLevel, &tileTextures);
	DrawPlayer();
	DrawTileDragging();
	DrawSelection();
	DrawMovingSelection();

	appContext.viewport->End();

	// UI drawing + viewport presentation
	BeginDrawing();

	appContext.viewport->Draw();
	DrawUI();

	EndDrawing();
}

void EditorScreen::Init() {
	// Implement any initialization logic for the editor screen here
	LoadTileTextures();
	LoadTileData();

	topBar = make_unique<TopBar>(this);

	//set player data position to player start position in level
	appContext.playerData.position = appContext.currentLevel->GetPlayerStartPos();
}

void EditorScreen::LoadTileTextures() {
	// load all tile textures to use in editor, referencing tileTextures.txt for file paths and texture ID's ignoring lines in tileTextures.txt that start with # as comments
	ifstream file("Assets/TileTextures.txt");

	if (!file.is_open()) {
		cerr << "Failed to open tile textures file!" << endl;
		return;
	}

	string line;

	while (getline(file, line)) {
		if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

		istringstream iss(line);

		int id;

		string path;

		if (!(iss >> id >> path)) {
			cerr << "Invalid line format in tile textures file: " << line << endl;
			continue;
		}

		// add extra path for textures folder
		path = "Assets/Textures/" + path;
		Texture2D texture = LoadTexture(path.c_str());

		if (texture.id == 0) {
			cerr << "Failed to load texture: " << path << endl;
			continue;
		}
		else
		{
			cout << "Loaded texture ID " << id << " from " << path << endl;
		}

		tileTextures[id] = texture;
	}
}

void EditorScreen::LoadTileData() {
	//empty tileList before loading new data
	tileList.clear();
	tileSelector->Clear();
	// load tile data from TileData.txt for tile ID's and associated Textures (exists so that different tiles can use the same texture with different properties
	ifstream file("Assets/TileData.txt");

	if (!file.is_open()) {
		cerr << "Failed to open tile data file!" << endl;
		return;
	}

	string line;

	while (getline(file, line)) {
		cout << "RAW LINE: [" << line << "]\n";
		if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
		istringstream iss(line);
		int id; //tile id
		int tid; //texture id
		string name; //tile name (not used in editor but can be used for reference)

		if (!(iss >> id >> tid >> name)) {
			cerr << "Invalid line format in tile data file: " << line << endl;
			continue;
		}
		else
		{
			cout << "Loaded tile data - ID: " << id << ", Texture ID: " << tid << ", Name: " << name << endl;
		}
		//store tile data in tileList for reference when adding tiles in editor
		tileList.push_back({ id, tid, WHITE });
		//add to tileSelector dropdown
		tileSelector->AddItem(id, name);
	}
	appContext.currentLevel->SetTileData(&tileList); //pass reference of tileList to level for reference when adding tiles in level
}

void EditorScreen::AddToUndoStack() {
	// Implement logic to add current level state to undo stack
	LevelSnapshot snapshot;
	snapshot.tileList = appContext.currentLevel->GetTileList();
	snapshot.playerStartPos = appContext.currentLevel->GetPlayerStartPos();
	appContext.undoStack.push_back(snapshot);
	//clear redo stack when new action is taken
	appContext.redoStack.clear();
}

void EditorScreen::Undo() {
	// Implement undo logic here
	if (!appContext.undoStack.empty()) {
		LevelSnapshot snapshot = appContext.undoStack.back();
		appContext.undoStack.pop_back();
		appContext.redoStack.push_back({ appContext.currentLevel->GetTileList(), appContext.currentLevel->GetPlayerStartPos() }); // Save current state to redo stack
		appContext.currentLevel->SetTileData(&tileList); // Ensure level has reference to tile data for drawing
		appContext.currentLevel->SetPlayerStartPos(snapshot.playerStartPos);
		appContext.playerData.position = snapshot.playerStartPos; // Update player position in editor to match snapshot
		for (size_t i = 0; i < snapshot.tileList.size(); i++) {
			const Tile& tile = snapshot.tileList[i];
			int x = i % appContext.currentLevel->GetWidth();
			int y = i / appContext.currentLevel->GetWidth();
			appContext.currentLevel->SetTile(x, y, tile);
		}
	}
}

void EditorScreen::Redo() {
	// Implement redo logic here
	if (!appContext.redoStack.empty()) {
		LevelSnapshot snapshot = appContext.redoStack.back();
		appContext.redoStack.pop_back();
		appContext.undoStack.push_back({ appContext.currentLevel->GetTileList(), appContext.currentLevel->GetPlayerStartPos() }); // Save current state to undo stack
		appContext.currentLevel->SetTileData(&tileList); // Ensure level has reference to tile data for drawing
		appContext.currentLevel->SetPlayerStartPos(snapshot.playerStartPos);
		appContext.playerData.position = snapshot.playerStartPos; // Update player position in editor to match snapshot
		for (size_t i = 0; i < snapshot.tileList.size(); i++) {
			const Tile& tile = snapshot.tileList[i];
			int x = i % appContext.currentLevel->GetWidth();
			int y = i / appContext.currentLevel->GetWidth();
			appContext.currentLevel->SetTile(x, y, tile);
		}
	}
}

void EditorScreen::KeyShortcuts() {
	// Implement keyboard shortcuts logic here (e.g., Ctrl+Z for undo, Ctrl+Y for redo)
	if (IsKeyDown(KEY_LEFT_CONTROL)) {
		if (IsKeyPressed(KEY_Z)) {
			if (IsKeyDown(KEY_LEFT_SHIFT)) {
				Redo();
			}
			else {
				Undo();
			}
		}
		if (IsKeyPressed(KEY_Y)) {
			Redo();
		}

		if (selection.active) {
			if (IsKeyPressed(KEY_D)) 
			{
				DeleteSelectedTiles();
			}
		}
	}
}

void EditorScreen::SaveLevelAs() {
	// Implement save level as logic here
	const char* filters[] = { "*.txt" };
	const char* path = tinyfd_saveFileDialog(
		"Save Level As",
		"Levels/NewLevel.txt",
		1,
		filters,
		"Text Files (*.txt)"
	);
	if (path) {
		string finalPath = path;

		if (finalPath.size() < 4 || finalPath.substr(finalPath.size() - 4) != ".txt") {
			finalPath += ".txt";
		}

		SaveLevelToFile(finalPath);
		appContext.currentLevel->SetLevelPath(finalPath); //store path for future saves
	}
	else {
		cerr << "Save cancelled or failed." << endl;
	}
}

void EditorScreen::SaveOpenLevel() {
	// Implement save open level logic here
	if (!appContext.currentLevel->GetLevelPath().empty()) {
		SaveLevelToFile(appContext.currentLevel->GetLevelPath());
	}
	else {
		cerr << "Saving to new File" << endl;
		SaveLevelAs();
	}
}

void EditorScreen::SaveLevelToFile(const string& filePath) {
	// Implement save level to file logic here
	if (appContext.currentLevel) {
		appContext.currentLevel->SaveToFile(filePath);
		cout << "Level saved to " << filePath << endl;
	}
	else {
		cerr << "No level to save!" << endl;
	}
}

void EditorScreen::LoadLevel() {
	// Implement load level logic here
	const char* filters[] = { "*.txt" };
	const char* path = tinyfd_openFileDialog(
		"Load Level",
		"Levels/",
		1,
		filters,
		"Text Files (*.txt)",
		0
	);
	if (path) {
		string finalPath = path;
		appContext.currentLevel->LoadFromFile(finalPath);
		appContext.currentLevel->SetLevelPath(finalPath); //store path for future saves
		Init(); //reinitialize editor screen to load new level data and textures
		cout << "Level loaded from " << finalPath << endl;
	}
	else {
		cerr << "Load cancelled or failed." << endl;
	}
}