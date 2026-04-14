#ifndef EDITORTOPBAR_H
#define EDITORTOPBAR_H

#include "GeneralFunctions.h"

class EditorScreen;

class MenuItem {
public:
	string label;
	function<void()> action;

	MenuItem(string label, function<void()> action) : label(label), action(action) {}

	void Update(Rectangle rect);
	void Draw(Rectangle rect) const;
};

class MenuBarButton {
public:

	Rectangle bounds;
	string label;
	bool isOpen = false;

	vector<MenuItem> menuItems;

	//constructor
	MenuBarButton(string label, Vector2 pos, Vector2 size);
	//default constructor
	MenuBarButton() : label(""), bounds({ 0,0,0,0 }) {}
	//destructor
	~MenuBarButton();

	void Update(Vector2 mousePos);
	void Draw() const;
	void DrawDropdown() const;

	void AddMenuItem(string label, function<void()> action) { menuItems.emplace_back(label, action); }
};

class TopBar {
	public:
	vector<MenuBarButton> menuButtons;
	
	TopBar(EditorScreen* editorScreen);
	TopBar() = default; // Default constructor
	void Update(Vector2 mousePos);
	void Draw() const;
};

#endif