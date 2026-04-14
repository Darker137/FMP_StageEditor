#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include "GeneralFunctions.h"
#include "Screen.h"

class GameplayScreen : public Screen {
private:
	unique_ptr<Player> player = nullptr;
	void DrawUI();
	void SwitchToEditor();
public:
	GameplayScreen(AppContext& context);
	~GameplayScreen() override = default;
	void Update() override;
	void Draw() override;
	void Init() override;
	void LoadTileTextures() override;
	void LoadTileData() override;
};

#endif
