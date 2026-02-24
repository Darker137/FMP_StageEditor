#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "GeneralFunctions.h"
#include "LevelClass.h"
#include "Viewport.h"
#include "Player.h"

struct AppContext {
	unique_ptr<Level> currentLevel = nullptr;
	unique_ptr<Viewport> viewport = nullptr;
	PlayerData playerData;
};
#endif