#include "GameplayScreen.h"

GameplayScreen::GameplayScreen(AppContext& context)
	: Screen(context) {
	player = Player::Create(appContext.playerData);
	appContext.viewport->SetCameraOffset({(float)(GetScreenWidth() / 2),(float)(GetScreenHeight() / 2)});
	appContext.viewport->SetCameraZoom(1.0f);
}

void GameplayScreen::DrawUI() {

}

void GameplayScreen::SwitchToEditor() {
	if (IsKeyPressed(KEY_TAB))
	{
		switchScreen = true;
	}
}

void GameplayScreen::Update() {
	float dt = GetFrameTime();
	player->Update(dt);
	appContext.currentLevel->PlayerTileCollision(player->GetData());

	Vector2 playerCenter = {
		player->GetData().position.x + player->GetData().size.x * 0.5f,
		player->GetData().position.y + player->GetData().size.y * 0.5f
	};
	appContext.viewport->UpdateCameraGameplay(dt, playerCenter);
	SwitchToEditor();
}

void GameplayScreen::Draw() {
	BeginDrawing();

	appContext.viewport->Begin();
	// Clear Viewport Background
	ClearBackground({ 50,50,50 ,225 });

	// Draw Level
	if (appContext.currentLevel) {
		appContext.viewport->DrawLevel(*appContext.currentLevel);
	}
	// Draw Player
	if (player) {
		player->Draw();
	}
	appContext.viewport->End();

	EndDrawing();
}