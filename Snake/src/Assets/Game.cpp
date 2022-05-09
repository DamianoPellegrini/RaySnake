#include "pch.h"
#include "Game.h"
#include "Utils/Loader.h"

Game* Game::s_Instance = nullptr;

Game::Game()
	: m_Playground(nullptr), m_Player(nullptr), m_CurrentPlacedApple(nullptr),
	m_Score(0), m_HiScore(0), m_TickRate(7), m_TickCount(0), m_TileDimension(16), m_GameState(GameState::MAIN_MENU) {
	Game::s_Instance = this;
}

Game* Game::Get() {
	if (s_Instance == nullptr)
		return new Game();

	return s_Instance;
}

void Game::Init() {
	SetExitKey(KEY_F2);
	ResetGame();

	SetTextureFilter(GetFont("Helvetica").texture, FILTER_ANISOTROPIC_16X);
	SetTextureFilter(GetFont("Roboto-Regular").texture, FILTER_ANISOTROPIC_16X);

	unsigned int data = 0;
	std::ifstream ifs("data.bin", std::ios::in | std::ios::binary);
	ifs.read((char*)&data, sizeof(unsigned int));
	m_HiScore = data;
	ifs.close();
}

void Game::ResetGame() {
	if (m_Playground != nullptr)
		delete m_Playground;
	if (m_Player != nullptr)
		delete m_Player;
	if (m_CurrentPlacedApple != nullptr)
		delete m_CurrentPlacedApple;

	m_Score = 0;
	m_Playground = new TiledMap(GetTexture("tile_bg"), GetScreenWidth() / (int)m_TileDimension, GetScreenHeight() / (int)m_TileDimension);
	m_Player = new Snake({
		m_TileDimension * (int)(m_Playground->GetWidth() / 2),
		m_TileDimension * (int)(m_Playground->GetHeight() / 2),
		m_TileDimension,m_TileDimension
		}, GetTexture("tile_snake"), GetTexture("tile_snaket")
	);

	m_CurrentPlacedApple = new Apple({ 0,0 }, { m_TileDimension ,m_TileDimension }, GetTexture("tile_apple"));
	m_CurrentPlacedApple->RandomizePosition(*m_Playground, *m_Player);

	//Adapt the window to map size
	SetWindowSize(m_Playground->GetWidth() * m_Playground->GetTileDimension(), m_Playground->GetHeight() * m_Playground->GetTileDimension());
}

void Game::Free() {
	delete m_Playground;
	delete m_Player;
	delete m_CurrentPlacedApple;

	unsigned int data = m_HiScore;
	std::ofstream ofs("data.bin", std::ios::out | std::ios::binary | std::ios::trunc);
	ofs.write((char*)&data, sizeof(unsigned int));
	ofs.close();
}

void Game::Run() {
	BeginDrawing();
	ClearBackground({ 245,245,245,255 });

	switch (m_GameState) {
	case GameState::MAIN_MENU:
		MainMenu();
		break;
	case GameState::PAUSE_MENU:
		PauseMenu();
		break;
	case GameState::GAME:
		GameMenu();
		break;
	}

#ifdef PROFILING
	const Font& profilingFont = GetFont("Roboto-Regular");
	const float fontSize = 24;
	const Vector2& size = MeasureTextEx(profilingFont, "Frametime: 0.0000000000 s", fontSize, 0.0f);
	const Vector2& textOffset = { GetScreenWidth() - size.x - 10,10 };
	const float padding = 5;

	DrawRectangleRec({ textOffset.x - padding,textOffset.y - padding,size.x + (fontSize / 2),(size.y * 2) + (fontSize / 2) }, { 0,0,0,136 });
	DrawTextEx(profilingFont, FormatText("FPS: %i", GetFPS()), textOffset, fontSize, 0.0f, WHITE);
	DrawTextEx(profilingFont, FormatText("Frametime: %.010f s", GetFrameTime()), { textOffset.x,textOffset.y + fontSize }, fontSize, 0.0f, WHITE);
#endif
	EndDrawing();
}

#pragma warning ( push )
#pragma warning (disable : 4244)
void Game::MainMenu() {
	//######################################
	//	Keybinds
	//######################################
	if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		m_GameState = GameState::GAME;
		ResetGame();
	}
	if (IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
		m_GameState = GameState::EXIT;

	Vector2 txtSize = MeasureTextEx(GetFont("Roboto-Regular"), "SNAKE", 96, 0.0f);
	Vector2 enterSize = MeasureTextEx(GetFont("Roboto-Regular"), "Press \"Enter\" to start a game.", 24, 0.0f);
	Vector2 exitSize = MeasureTextEx(GetFont("Roboto-Regular"), "Press \"Escape\" to exit.", 24, 0.0f);

	float xOffset = 80;

	//######################################
	//	Background
	//######################################
	DrawTextureRec(GetTexture("tile_snake"), { 0,0,GetScreenWidth() + .0f,GetScreenHeight() + .0f }, { 0,0 }, { 150,150,150,255 });
	DrawRectangle(
		(GetScreenWidth() - txtSize.x - xOffset * 2) / 2,	//X
		(GetScreenHeight() / 8) - (txtSize.y / 2),	//Y
		txtSize.x + xOffset * 2,								//Width
		(((GetScreenWidth() - enterSize.y) / 2) + 13) - ((GetScreenHeight() / 8) - (txtSize.y / 2)) + 30.0f,	//Height
		{ 0,0,0,136 });

	//######################################
	//	Text
	//######################################
	DrawTextEx(GetFont("Roboto-Regular"), "SNAKE", { (GetScreenWidth() - txtSize.x) / 2, (GetScreenHeight() / 8) - (txtSize.y / 2) }, 96, 0.0f, WHITE);
	DrawTextEx(GetFont("Roboto-Regular"), "Press \"Enter\" to start a game.", { (GetScreenWidth() - enterSize.x) / 2, ((GetScreenWidth() - enterSize.y) / 2) - 13 }, 24, 0.0f, WHITE);
	DrawTextEx(GetFont("Roboto-Regular"), "Press \"Escape\" to exit.", { (GetScreenWidth() - exitSize.x) / 2, ((GetScreenWidth() - enterSize.y) / 2) + 13 }, 24, 0.0f, WHITE);
}

void Game::PauseMenu() {
	//######################################
	//	Keybinds
	//######################################
	if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		m_GameState = GameState::GAME;
	}
	if (IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
		m_GameState = GameState::MAIN_MENU;

	Vector2 txtSize = MeasureTextEx(GetFont("Roboto-Regular"), "PAUSE", 96, 0.0f);
	Vector2 enterSize = MeasureTextEx(GetFont("Roboto-Regular"), "Press \"Enter\" to resume the game.", 24, 0.0f);
	Vector2 exitSize = MeasureTextEx(GetFont("Roboto-Regular"), "Press \"Escape\" to return to the menu.", 24, 0.0f);

	float xOffset = 80;

	//######################################
	//	Background
	//######################################
	DrawTextureRec(GetTexture("tile_snake"), { 0,0,GetScreenWidth() + .0f,GetScreenHeight() + .0f }, { 0,0 }, { 150,150,150,255 });
	DrawRectangle(
		(GetScreenWidth() - exitSize.x - xOffset * 2) / 2,
		(GetScreenHeight() / 8) - (txtSize.y / 2),
		exitSize.x + xOffset * 2,
		(((GetScreenWidth() - enterSize.y) / 2) + 13) - ((GetScreenHeight() / 8) - (txtSize.y / 2)) + 30,
		{ 0,0,0,136 });

	//######################################
	//	Text
	//######################################
	DrawTextEx(GetFont("Roboto-Regular"), "PAUSE", { (GetScreenWidth() - txtSize.x) / 2, (GetScreenHeight() / 8) - (txtSize.y / 2) }, 96, 0.0f, WHITE);
	DrawTextEx(GetFont("Roboto-Regular"), "Press \"Enter\" to resume the game.", { (GetScreenWidth() - enterSize.x) / 2, ((GetScreenWidth() - enterSize.y) / 2) - 13 }, 24, 0.0f, WHITE);
	DrawTextEx(GetFont("Roboto-Regular"), "Press \"Escape\" to return to the menu.", { (GetScreenWidth() - exitSize.x) / 2, ((GetScreenWidth() - enterSize.y) / 2) + 13 }, 24, 0.0f, WHITE);
}
#pragma warning ( pop )

void Game::GameMenu() {
	//######################################
	//	Keybinds
	//######################################
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_UP))
		m_Player->ChangeDirection(Direction::UP);
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
		m_Player->ChangeDirection(Direction::DOWN);
	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
		m_Player->ChangeDirection(Direction::LEFT);
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
		m_Player->ChangeDirection(Direction::RIGHT);
	if ((IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_RIGHT_FACE_UP)) && (m_Score >= m_HiScore || FileExists("rainbow.dev"))) {
		m_Player->ToggleRainbow();
		m_Playground->ToggleDarkMode();
	}
	if (IsKeyPressed(KEY_ESCAPE) || IsGamepadButtonPressed(GAMEPAD_PLAYER1, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
		m_GameState = GameState::PAUSE_MENU;
		return;
	}

	//######################################
	//	Game logic
	//######################################
	if (m_TickCount > (1 / m_TickRate)) {
		m_TickCount = 0;
		m_Player->Tick();
	}

	m_TickCount += GetFrameTime();

	//######################################
	//	Score logic
	//######################################
	if (m_Player->IsCollidingWithApple(*m_CurrentPlacedApple)) {
		SLOG_INFO("Colliding with apple");
		m_CurrentPlacedApple->RandomizePosition(*m_Playground, *m_Player);
		m_Player->AddPiece();
		m_Score++;
		if (m_Score == m_HiScore) {
			m_Player->ToggleRainbow();
			m_Playground->ToggleDarkMode();
		}
	}

	//######################################
	//	Lose logic
	//######################################
	if (m_Player->IsCollidingWithBorder(*m_Playground) || m_Player->IsCollidingWithSelf()) {
		SLOG_INFO("Colliding with either self or wall");
		if (m_Score > m_HiScore)
			m_HiScore = m_Score;
		m_GameState = GameState::MAIN_MENU;
		return;
	}

	m_Playground->Render();
	m_Player->Render();
	m_CurrentPlacedApple->Render();

	//######################################
	//	Score Overlay
	//######################################
	const Font& scoreFont = GetFont("Helvetica");
	const float scoreFontSize = 12;
	const Vector2& scoreBoxSize = MeasureTextEx(scoreFont, "Hi - Score: 0000", scoreFontSize, 0.0f);
	const Vector2& scoreTextOffset = { 10,10 };
	const float scoreTextPadding = 5;

	DrawRectangleRec({ scoreTextOffset.x - scoreTextPadding,scoreTextOffset.y - scoreTextPadding,scoreBoxSize.x + (scoreFontSize / 2),(scoreBoxSize.y * 2) + (scoreFontSize / 2) }, { 0,0,0,136 });
	DrawTextEx(scoreFont, FormatText("Score: %04i", m_Score), scoreTextOffset, scoreFontSize, 0.0f, WHITE);
	DrawTextEx(scoreFont, FormatText("Hi-Score: %04i", m_HiScore), { scoreTextOffset.x,scoreTextOffset.y + scoreFontSize }, scoreFontSize, 0.0f, WHITE);

	//######################################
	//	DEBUG Layer(Last to be on top of everything)
	//######################################
#ifdef DEBUG
	if (m_TickCount > (1 / m_TickRate) / 3.f) {
		if (IsKeyPressed(KEY_R)) {
			m_CurrentPlacedApple->RandomizePosition(*m_Playground, *m_Player);
			m_TickCount = 0.f;
		}
		if (IsKeyDown(KEY_T)) {
			m_Player->Tick();
			m_Player->AddPiece();
			m_Score++;
			m_TickCount = 0.f;
		}if (IsKeyDown(KEY_H)) {
			m_Player->Tick();
			m_TickCount = 0.f;
		}
	}
#endif
}