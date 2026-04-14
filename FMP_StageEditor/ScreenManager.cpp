#include "ScreenManager.h"
#include "TileClass.h"

ScreenManager::ScreenManager() {
	appContext.currentLevel = Level::GetLevel(0);
	currentScreen = make_unique<EditorScreen>(appContext);
}

ScreenManager::~ScreenManager() {
}

void ScreenManager::Init() {
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow((int)defaultScreenSize.x, (int)defaultScreenSize.y, "FMP Stage Editor");
	MaximizeWindow();

	appContext.viewport = Viewport::Create(defaultScreenSize.x, defaultScreenSize.y);

	currentScreen->Init();
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
	currentScreen->Init();
}

void ScreenManager::ToggleFullscreen() {
	if (IsKeyPressed(KEY_F11)){
		ToggleFullscreen();
	}
}

