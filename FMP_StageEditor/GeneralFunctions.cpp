#include "GeneralFunctions.h"

void DropdownList::AddItem(int id, string name) {
	labels.push_back({ id, name });
}

void DropdownList::Update() {
	Vector2 mousePos = GetMousePosition();

	//toggle open/close whe clicking main box
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(mousePos, bounds)) {
			isOpen = !isOpen;
			return;
		}

		//If open, check for item selection
		if(isOpen) {
			for (int i = 0; i < labels.size(); i++) {
				Rectangle itemRect = {
					bounds.x,
					bounds.y + bounds.height * (i + 1),
					bounds.width,
					bounds.height
				};
				if (CheckCollisionPointRec(mousePos, itemRect)) {
					selectedItem = labels[i];
					selectedIndex = i;
					isOpen = false;
					return;
				}
			}
			//Click outside closes dropdown
			isOpen = false;
		}
	}
}

void DropdownList::Draw() const {
	// Main Box
	DrawRectangleRec(bounds, LIGHTGRAY);
	DrawRectangleLinesEx(bounds, 1, BLACK);

	int fontSize = 20;

	if (!labels.empty()) {
		if (selectedItem.id == -1) 
		{
			// No selection yet, show placeholder
			DrawText(
				dropDownLabel.c_str(),
				(int)(bounds.x + 5),
				(int)(bounds.y + bounds.height / 2 - fontSize / 2),
				fontSize,
				DARKGRAY
			);
		}
		else
		{
			DrawText(
				selectedItem.name.c_str(),
				(int)(bounds.x + 5),
				(int)(bounds.y + bounds.height / 2 - fontSize / 2),
				fontSize,
				BLACK
			);
		}
	}

	// Arrow Indicator
	DrawText(isOpen ? "^" : "v",
		(int)(bounds.x + bounds.width - 20),
		(int)(bounds.y + bounds.height / 2 - MeasureText(isOpen ? "^" : "v", 20) / 2),
		20,
		BLACK
	);

	// Dropdown Items
	if (!isOpen) return;
	
	for(int i = 0; i < labels.size(); i++) {
		Rectangle itemRect = {
			bounds.x,
			bounds.y + bounds.height * (i + 1),
			bounds.width,
			bounds.height
		};
		Color bg = (i == selectedIndex) ? GRAY : LIGHTGRAY;
		DrawRectangleRec(itemRect, bg);
		DrawRectangleLinesEx(itemRect, 1, BLACK);
		DrawText(
			labels[i].name.c_str(),
			(int)(itemRect.x + 5),
			(int)(itemRect.y + itemRect.height / 2 - fontSize / 2 ),
			fontSize,
			BLACK
		);
	}
}

// -------------------------------- Button Class ----------------------------------

void Button::Update(Vector2 mousePos) {
	isPressed = false;
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(mousePos, bounds)) {
			isPressed = true;
		}
	}
}

void Button::Draw() const {
	// Draw button rectangle
	Color bgColor = isPressed ? DARKGRAY : LIGHTGRAY;
	DrawRectangleRec(bounds, bgColor);
	DrawRectangleLinesEx(bounds, 1, BLACK);
	// Draw button label
	int fontSize = 20;
	int textWidth = MeasureText(label.c_str(), fontSize);
	DrawText(
		label.c_str(),
		(int)(bounds.x + (bounds.width - textWidth) / 2),
		(int)(bounds.y + (bounds.height - fontSize) / 2),
		fontSize,
		BLACK
	);
}

// -------------------------------- Utility Functions ----------------------------------
