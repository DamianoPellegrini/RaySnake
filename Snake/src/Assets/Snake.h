#pragma once

class Apple;
class TiledMap;

enum class Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class Snake {
private:
	std::vector<Rectangle> m_Snake;
	Direction m_SnakeDirection;
	Texture2D m_TileTexture;
	Texture2D m_RainbowTileTex;
	Rectangle m_LastSnakePart;
	bool changedDirection;
	bool rainbowSnake;
	const Direction GetOppositeDirection(const Direction& dir) const;
public:
	Snake(Rectangle headPos, const Texture2D& tileTex, const Texture2D& rainbowTileTex);

	inline const Rectangle& GetSnakeHead() const { return m_Snake[0]; }

	inline void ToggleRainbow() { rainbowSnake = !rainbowSnake; }

	void Tick();
	void Render() const;

	bool IsCollidingWithAppleHead(const Apple& colliding) const;
	bool IsCollidingWithApple(const Apple& colliding) const;
	bool IsCollidingWithBorder(const TiledMap& colliding) const;
	bool IsCollidingWithSelf() const;

	void ChangeDirection(const Direction& dir);
	void AddPiece();
};
