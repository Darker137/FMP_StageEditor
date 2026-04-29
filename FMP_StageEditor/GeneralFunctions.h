#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#include <iostream> 			  // Required for console output
#include <cmath>                  // Required for math functions
#include <vector>                 // Required for using std::vector
#include <memory>                 // Required for smart pointers
#include <unordered_set>         // Required for using std::unordered_set
#include <unordered_map>         // Required for using std::unordered_map
#include <string>                 // Required for using std::string
#include <sstream> 			   // Required for string stream operations
#include <fstream> 				   // Required for file input/output operations
#include <functional>             // Required for std::function

#include "raygui.h"                 // Required for GUI controls
#include "raylib.h"                 //Main Raylib core library
#include "raymath.h"                //Raylib Math functions library
#include "rlgl.h"                   //RayLib OpenGL library

#include "tinyfiledialogs/tinyfiledialogs.h" // Required for file dialogs

using namespace std;

namespace EditorConfig {
	constexpr bool debugMode = true; // Toggle debug mode for additional output and visuals
};

//Grid Position Struct, similar to Vector2 but with int values
struct GridPos {
	int x;
	int y;
};;

struct DropdownItem {
	int id;
	string name;
};

// Dropdown List UI Control
class DropdownList {
public:
	Rectangle bounds;
	bool isOpen = false;
	string dropDownLabel;
	DropdownItem selectedItem;
	vector<DropdownItem> labels;
	int selectedIndex = -1;

	DropdownList() { selectedItem = { -1, "None" }, bounds = { 0,0,0,0 }; }
	void AddItem(int id, string name);
	void Update();
	void Draw() const;
	void Clear() { labels.clear(); selectedItem = { -1, "None" }; selectedIndex = -1; }
};

class Button {
	public:
	Rectangle bounds;
	string label;
	bool isPressed = false;

	Button() {
		bounds = { 0,0,0,0 }; label = "Button";
	}
	void Update(Vector2 mousePos);
	void Draw() const;
	bool IsClicked() const { return isPressed; };
};

#endif