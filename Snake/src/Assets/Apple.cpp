#include "pch.h"
#include "Apple.h"

Apple::Apple(int x, int y, int width, int height, const Texture2D& tex) {
	m_Tile = { .0f + x, .0f + y, .0f + width, .0f + height };
	m_TileTexture = tex;
}

Apple::Apple(Vector2 position, Vector2 dimension, const Texture2D& tex) {
	m_Tile = { position.x, position.y, dimension.x, dimension.y };
	m_TileTexture = tex;
}

Apple::Apple(Rectangle tile, const Texture2D& tex) {
	m_Tile = tile;
	m_TileTexture = tex;
}

void Apple::Render() const {
	DrawTextureRec(m_TileTexture, m_Tile, { m_Tile.x,m_Tile.y }, WHITE);
}

void Apple::RandomizePosition(const TiledMap& map, const Snake& snake) {
	m_Tile.x = .0f + GetRandomValue(0, map.GetWidth() - 1) * (int)(m_Tile.width);
	m_Tile.y = .0f + GetRandomValue(0, map.GetHeight() - 1) * (int)(m_Tile.height);
	if (snake.IsCollidingWithApple(*this)) RandomizePosition(map, snake);
}