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
		appContext.viewport->DrawLevel(*appContext.currentLevel, &tileTextures);
	}
	// Draw Player
	if (player) {
		player->Draw();
	}
	appContext.viewport->End();

	EndDrawing();
}

void GameplayScreen::Init() {
	appContext.currentLevel->RebuildUsedTileIDs();
	// Load tile textures and data for the current level
	LoadTileData();
	LoadTileTextures();
}

void GameplayScreen::LoadTileTextures() {
	//load tile textures that exist in the current level based on tile IDs used in the level's tiles, referencing tileTextures.txt for file paths and texture ID's ignoring lines in tileTextures.txt that start with # as comments
	ifstream file("Assets/TileTextures.txt");

	if (!file.is_open()) {
		cerr << "Failed to open tile textures file!" << endl;
		return;
	}

	string line;

	while (getline(file, line)) {
		if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments

		istringstream iss(line);
		int id;
		string path;

		if (!(iss >> id >> path)) {
			cerr << "Invalid line format in tile textures file: " << line << endl;
			continue;
		}
		//check if tile ID is used in the current level
		if (appContext.currentLevel->IsTileIDUsed(id)) {
			path = "Assets/Textures/" + path;
			Texture2D texture = LoadTexture(path.c_str());
			tileTextures[id] = texture;
			cout << "Loaded texture for Tile ID: " << id << " from path: " << path << endl;
		}
		else {
			cout << "Tile ID: " << id << " not used in current level, skipping texture load." << endl;
		}
	}
}

void GameplayScreen::LoadTileData() {
	// Implement any logic needed to load tile data for the current level here
	// load tile data from TileData.txt for ID's and textures
	// only saving the tile data that are used in the current level
	ifstream file("Assets/TileData.txt");

	if (!file.is_open()) {
		cerr << "Failed to open tile data file!" << endl;
		return;
	}

	string line;

	while (getline(file, line)) {
		cout << "RAW LINE: [" << line << "]\n";
		if (line.empty() || line[0] == '#') continue; // Skip empty lines and comments
		istringstream iss(line);
		int id; //tile id
		int tid; //texture id
		string name; //tile name (not used in gameplay but can be used for reference)
		if (!(iss >> id >> tid >> name)) {
			cerr << "Invalid line format in tile data file: " << line << endl;
			continue;
		}
		else
		{
			cout << "Loaded tile data - ID: " << id << ", Texture ID: " << tid << ", Name: " << name << endl;
		}
		if (!appContext.currentLevel->IsTileIDUsed(id)) {
			cout << "Tile ID: " << id << " not used in current level, skipping tile data load." << endl;
			continue;
		}
		appContext.currentLevel->AddToExistingTiles({ id, tid, WHITE });
	}
}