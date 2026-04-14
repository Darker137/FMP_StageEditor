#include "EditorTopBar.h"
#include "EditorScreen.h"

void MenuItem::Update(Rectangle rect) {
	Vector2 mousePos = GetMousePosition();
	if (CheckCollisionPointRec(mousePos, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		action(); // Execute the assigned action
	}
}

void MenuItem::Draw(Rectangle rect) const {
	Vector2 mousePos = GetMousePosition();
	Color bgColor = CheckCollisionPointRec(mousePos, rect) ? DARKGRAY : GRAY;
	DrawRectangleRec(rect, bgColor);
	//border
	DrawRectangleLinesEx(rect, 1, BLACK);
	DrawText(
		label.c_str(),
		(int)(rect.x + 5),
		(int)(rect.y + rect.height / 2 - 20 / 2),
		20,
		BLACK
	);
}

MenuBarButton::MenuBarButton(string label, Vector2 pos, Vector2 size) : label(label) {
	// Constructor logic if needed
	//use label to determine width of button if bounds width is 0
	bounds = { pos.x, pos.y, size.x, size.y };
	if (bounds.width == 0) {
		bounds.width = MeasureText(label.c_str(), 20) + 40; // 40 for padding
	}
}

MenuBarButton::~MenuBarButton() {
	// Destructor logic if needed
}

void MenuBarButton::Update(Vector2 mousePos) {
	if (CheckCollisionPointRec(mousePos, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		isOpen = !isOpen; // Toggle dropdown
	}
	if (isOpen) {
		for (size_t i = 0; i < menuItems.size(); ++i) {
			Rectangle itemRect = { bounds.x, bounds.y + bounds.height * (i + 1), bounds.width, bounds.height };
			menuItems[i].Update(itemRect);
		}
	}
}

void MenuBarButton::Draw() const {
	DrawRectangleRec(bounds, GRAY);
	//border
	DrawRectangleLinesEx(bounds, 1, BLACK);
	DrawText(
		label.c_str(),
		(int)(bounds.x + 5),
		(int)(bounds.y + bounds.height / 2 - 20 / 2),
		20,
		BLACK
	);
	if (isOpen) {
		DrawDropdown();
	}
}

void MenuBarButton::DrawDropdown() const {
	for (size_t i = 0; i < menuItems.size(); ++i) {
		Rectangle itemRect = { bounds.x, bounds.y + bounds.height * (i + 1), bounds.width, bounds.height };
		menuItems[i].Draw(itemRect);
	}
}

TopBar::TopBar(EditorScreen* editorScreen) {
	// Initialize menu buttons if needed
	//menu button width 0 so it will be set based on label length in MenuBarButton constructor
	MenuBarButton fileButton("File", { 0, 0, }, { 0, 30 });
	fileButton.AddMenuItem("New", [editorScreen]() { editorScreen->NewLevel(); });
	fileButton.AddMenuItem("Save As", [editorScreen]() { editorScreen->SaveLevelAs(); });
	fileButton.AddMenuItem("Save", [editorScreen]() { editorScreen->SaveOpenLevel(); });
	fileButton.AddMenuItem("Load", [editorScreen]() { editorScreen->LoadLevel(); });
	menuButtons.push_back(fileButton);
}

void TopBar::Update(Vector2 mousePos) {
	for (MenuBarButton& button : menuButtons) {
		button.Update(mousePos);
	}
}

void TopBar::Draw() const {
	for (const MenuBarButton& button : menuButtons) {
		button.Draw();
	}
}