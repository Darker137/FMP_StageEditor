#include "EditorScreen.h"

EditorScreen::EditorScreen(AppContext& context)
	: Screen(context) {

	dragStartPos = { 0,0 };
	dragWorldPos = { 0.0f, 0.0f };
	playerStartPos = { 0.0f, 0.0f };

	tileSelector = make_unique<DropdownList>();
	tileSelector->dropDownLabel = "Select Tile";
	tileSelector->bounds = { 10, 10, 150, 30 };
	tileSelector->AddItem(0, "Empty Tile");
	tileSelector->AddItem(1, "Tile 1");
	tileSelector->AddItem(2, "Tile 2");
	tileSelector->AddItem(3, "Tile 3");

	addButton = make_unique<Button>();
	addButton->label = "Add Tile";
	addButton->bounds = { 170, 10, 100, 30 };

	playButton = make_unique<Button>();
	playButton->label = "Play";
	playButton->bounds = { 280, 10, 100, 30 };

}

EditorScreen::~EditorScreen() {
}

void EditorScreen::UpdateViewportInput() {
	// Implement viewport input update logic here
}

void EditorScreen::UpdateTileDragging() {
	// Implement tile dragging update logic here
	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());
	GridPos grid = appContext.viewport->WorldToGrid(mouseWorld);

	//Start Dragging
	if (!isDraggingTile && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (appContext.currentLevel->InBounds(grid.x, grid.y)) {
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

void EditorScreen::UpdateMovePlayer() {
	// Implement player movement update logic here
	Vector2 mouseWorld = appContext.viewport->ScreenToWorld(GetMousePosition());
	Rectangle playerRect = {
		appContext.playerData.position.x,
		appContext.playerData.position.y,
		appContext.playerData.size.x,
		appContext.playerData.size.y
	};
	if (CheckCollisionPointRec(mouseWorld, playerRect) && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
		if (!isMovingPlayer)
		{
			playerStartPos = appContext.playerData.position;
		}
		isMovingPlayer = true;
	}
	if (isMovingPlayer && IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
		if (!appContext.currentLevel->PlayerInBounds(appContext.playerData)) {
			appContext.playerData.position = playerStartPos;
		}
		isMovingPlayer = false;
	}
	if (isMovingPlayer) {
		appContext.playerData.position = mouseWorld;
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

		appContext.viewport->DrawTile(&draggedTile, appContext.viewport->ScreenToWorld(GetMousePosition()));
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
}

void EditorScreen::AddTile() {
	// Implement add tile logic here
	if (addButton->IsClicked() && tileSelector->selectedItem.id != -1) {
		cout << "Adding Tile ID: " << tileSelector->selectedItem.id << endl;
		// Additional logic to add the selected tile can be implemented here
		draggedTile = tileList[tileSelector->selectedItem.id];
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
	// Implement editor screen update logic here
	appContext.viewport->UpdateCameraEditor(GetFrameTime());
	UpdateTileDragging();
	tileSelector->Update();
	addButton->Update();
	playButton->Update();
	AddTile();
	PlayButton();
	UpdateMovePlayer();
}

void EditorScreen::Draw() {

	// Viewport drawing
	appContext.viewport->Begin();

	ClearBackground({ 10,10,10 ,225 });

	appContext.viewport->DrawBoundsBackground(*appContext.currentLevel);
	appContext.viewport->DrawGrid();
	appContext.viewport->DrawBounds(*appContext.currentLevel);
	appContext.viewport->DrawLevel(*appContext.currentLevel);
	DrawPlayer();
	DrawTileDragging();

	appContext.viewport->End();

	// UI drawing + viewport presentation
	BeginDrawing();

	appContext.viewport->Draw();
	DrawUI();

	EndDrawing();
}