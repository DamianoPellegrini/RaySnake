#pragma once

#include "Assets/TiledMap.h"
#include "Assets/Snake.h"
#include "Assets/Apple.h"

enum class GameState {
	MAIN_MENU,
	PAUSE_MENU,
	GAME,
	EXIT
};

class Game {
public:
	static Game* Get();

	void Init();	//Allocates all the game elements in memory
	void Free();	//Frees the memory used by the initialization process
	void Run();		//Render, Points, Physics, should be called once every frame

	inline const Texture2D& GetTexture(std::string name) const { return m_Textures.at(name); }
	inline const Font& GetFont(std::string name) const { return m_Fonts.at(name); }

	inline bool ShouldGameEnd() { return WindowShouldClose() || m_GameState == GameState::EXIT; }
private:
	friend class Loader;
	static Game* s_Instance;

	std::map<std::string, Texture2D> m_Textures;
	std::map<std::string, Font> m_Fonts;

	float m_TickCount;
	const float m_TickRate;
	float m_TileDimension;
	unsigned int m_Score;
	unsigned int m_HiScore;
	GameState m_GameState;

	TiledMap* m_Playground;
	Snake* m_Player;
	Apple* m_CurrentPlacedApple;

	Game();

	void ResetGame();

	void MainMenu();
	void PauseMenu();
	void GameMenu();
};