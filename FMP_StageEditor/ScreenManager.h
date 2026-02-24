#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "GeneralFunctions.h"
#include "EditorScreen.h"
#include "GameplayScreen.h"

class ScreenManager {
private:
	ScreenManager();

	const Vector2 defaultScreenSize = { 1280, 720 };
	unique_ptr<Screen> currentScreen = nullptr;
	bool inEditorMode = true;
	AppContext appContext;

public:
	static unique_ptr<ScreenManager> Create() {
		return unique_ptr<ScreenManager>(new ScreenManager());
	}
	~ScreenManager();
	void Init();
	void Update();
	void Draw();
	void SwitchScreen();
	void ToggleFullscreen();
};

#endif