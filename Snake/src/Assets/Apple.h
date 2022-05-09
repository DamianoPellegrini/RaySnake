#pragma once

#include"Assets/TiledMap.h"
#include"Assets/Snake.h"

class Apple {
private:
	Rectangle m_Tile;
	Texture2D m_TileTexture;
public:

	Apple(int x, int y, int width, int height, const Texture2D& tileTexture);
	Apple(Vector2 position, Vector2 dimension, const Texture2D& tileTexture);
	Apple(Rectangle tile, const Texture2D& tileTexture);

	inline const Rectangle& GetCollider() const { return m_Tile; }

	void Render() const;

	void RandomizePosition(const TiledMap& map, const Snake& snake);
};
