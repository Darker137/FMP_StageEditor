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
	Screen(AppContext& context) : appContext(context) {}
	virtual ~Screen() = default;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// Getter for switchScreen
	bool ShouldSwitchScreen() const { return switchScreen; }
};

#endif