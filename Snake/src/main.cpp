#include "pch.h"

#include "Logger/Logger.h"
#include "Assets/Game.h"
#include "Utils/Loader.h"

#include <raylib.h>

#define VERSION "1.3"

int main() {
	SLOG_TRACE("Snake");
#ifdef DEBUG
	// SetConfigFlags(FLAG_VSYNC_HINT);
#endif
	InitWindow(16 * 31, 16 * 31, "Snake | " VERSION " | Damiano Pellegrini");

	const Image& icon = LoadImage("resources/textures/tile_snake.png");
	SetWindowIcon(icon);

	//SetTargetFPS(144);

	Loader::Get()->Load(*Game::Get());
	Game::Get()->Init();

	while (!Game::Get()->ShouldGameEnd()) {
		Game::Get()->Run();
	}

	Game::Get()->Free();
	Loader::Get()->Unload(*Game::Get());

	UnloadImage(icon);

	CloseWindow();

	return 0;
}