#ifndef SCREEN_H
#define SCREEN_H

#include "GeneralFunctions.h"
#include "AppContext.h"

// Abstract Screen Class
class Screen {
protected:
	AppContext& appContext;
	bool switchScreen = false;
public:

	//tile textures stored here with tile ID as index, used to draw tiles without storing texture info in each tile
	unordered_map<int, Texture2D> tileTextures;
	//string to store path to opened level for saving purposes

	Screen(AppContext& context) : appContext(context) {}
	virtual ~Screen() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Init() {} // Optional initialization function for screens that need it
	virtual void LoadTileTextures() = 0;
	virtual void LoadTileData() = 0;

	// Getter for switchScreen
	bool ShouldSwitchScreen() const { return switchScreen; }
};

#endif