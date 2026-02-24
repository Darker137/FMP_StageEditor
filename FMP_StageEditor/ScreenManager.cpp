#include "ScreenManager.h"

ScreenManager::ScreenManager() {
	appContext.currentLevel = Level::GetLevel(0);
	currentScreen = make_unique<EditorScreen>(appContext);

	appContext.currentLevel->SetTile(0, 3, Tile{ 1, true });
	appContext.currentLevel->SetTile(1, 3, Tile{ 1, true });
	appContext.currentLevel->SetTile(2, 3, Tile{ 1, true });
	appContext.currentLevel->SetTile(3, 3, Tile{ 1, true });
}

ScreenManager::~ScreenManager() {
}

void ScreenManager::Init() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow((int)defaultScreenSize.x, (int)defaultScreenSize.y, "FMP Stage Editor");
	MaximizeWindow();

	appContext.viewport = Viewport::Create(defaultScreenSize.x, defaultScreenSize.y);
}

void ScreenManager::Update() {
	currentScreen->Update();
	if(currentScreen->ShouldSwitchScreen()) {
		SwitchScreen();
	}
}

void ScreenManager::Draw() {

	ClearBackground(RAYWHITE);
	DrawText("It works!", 190, 200, 20, DARKGRAY);

	currentScreen->Draw();
}

void ScreenManager::SwitchScreen() {
	inEditorMode = !inEditorMode;
	if (inEditorMode) {
		appContext.viewport->SetViewportMode(ViewportMode::Editor);
		currentScreen = make_unique<EditorScreen>(appContext);
	}
	else {
		appContext.viewport->SetViewportMode(ViewportMode::Gameplay);
		currentScreen = make_unique<GameplayScreen>(appContext);
	}
}

void ScreenManager::ToggleFullscreen() {
	if (IsKeyPressed(KEY_F11)){
		ToggleFullscreen();
	}
}