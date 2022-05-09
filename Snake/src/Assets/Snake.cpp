#include "pch.h"
#include "Snake.h"

#include "Apple.h"
#include "TiledMap.h"

const Direction Snake::GetOppositeDirection(const Direction& dir) const {
	switch (dir) {
	case Direction::UP:
		return Direction::DOWN;
	case Direction::DOWN:
		return Direction::UP;
	case Direction::LEFT:
		return Direction::RIGHT;
	case Direction::RIGHT:
		return Direction::LEFT;
	}
	return Direction::UP;
}

Snake::Snake(Rectangle headPos, const Texture2D& tileTex, const Texture2D& rainbowTileTex)
	: m_SnakeDirection(Direction::UP), m_TileTexture(tileTex), m_RainbowTileTex(rainbowTileTex),
	changedDirection(false), m_LastSnakePart({ 0,0,0,0 }), rainbowSnake(false) {
	m_Snake.reserve(120);
	m_Snake.push_back(headPos);
	m_Snake.push_back({ headPos.x,headPos.y + headPos.width, headPos.width, headPos.height });
	m_Snake.push_back({ headPos.x,headPos.y + (headPos.width * 2), headPos.width, headPos.height });
}

void Snake::Tick() {
	m_LastSnakePart = m_Snake[0];
	switch (m_SnakeDirection) {
	case Direction::UP:
		m_Snake[0].y -= m_Snake[0].height;
		break;
	case Direction::DOWN:
		m_Snake[0].y += m_Snake[0].height;
		break;
	case Direction::LEFT:
		m_Snake[0].x -= m_Snake[0].width;
		break;
	case Direction::RIGHT:
		m_Snake[0].x += m_Snake[0].width;
		break;
	}
	for (size_t i = 1; i < m_Snake.size(); i++) {
		Rectangle& r = m_Snake[i];
		Rectangle temp = Rectangle(r);
		r.x = m_LastSnakePart.x;
		r.y = m_LastSnakePart.y;
		m_LastSnakePart = temp;
	}
	changedDirection = false;
}

void Snake::Render() const {
	for (unsigned int i = 0; i < m_Snake.size(); i++) {
		//if (m_Snake[i].x == m_Snake[0].x && m_Snake[i].y == m_Snake[0].y)
		//	DrawTextureRec(m_TileTexture, m_Snake[i], { m_Snake[i].x,m_Snake[i].y }, GREEN);
		//else
		if (rainbowSnake) {
			Color c = ColorFromHSV({ ((i / 100.0f) * 360) * 2.25f, 1, 1 });
			DrawTextureRec(m_RainbowTileTex, m_Snake[i], { m_Snake[i].x,m_Snake[i].y }, c);
		} else {
			DrawTextureRec(m_TileTexture, m_Snake[i], { m_Snake[i].x,m_Snake[i].y }, WHITE);
			//DrawTextureRec(m_TileTexture, m_Snake[i], { m_Snake[i].x,m_Snake[i].y }, ColorFromHSV({255, 255, (i / .0f + m_Snake.size()) * 255 }));
		}
	}
}

bool Snake::IsCollidingWithAppleHead(const Apple& colliding) const {
	return m_Snake[0].x == colliding.GetCollider().x && m_Snake[0].y == colliding.GetCollider().y;
}

bool Snake::IsCollidingWithApple(const Apple& colliding) const {
	for (auto const& it : m_Snake) {
		if (it.x == colliding.GetCollider().x && it.y == colliding.GetCollider().y)
			return true;
	}
	return false;
}

bool Snake::IsCollidingWithBorder(const TiledMap& colliding) const {
	return
		m_Snake[0].x >= colliding.GetWidth() * colliding.GetTileDimension() ||
		m_Snake[0].y >= colliding.GetHeight() * colliding.GetTileDimension() ||
		m_Snake[0].x < 0 ||
		m_Snake[0].y < 0;
}

bool Snake::IsCollidingWithSelf() const {
	for (size_t i = 1; i < m_Snake.size(); i++) {
		if (m_Snake[i].x == m_Snake[0].x && m_Snake[i].y == m_Snake[0].y)
			return true;
	}
	return false;
}

void Snake::ChangeDirection(const Direction& dir) {
	const Direction oppositeDir = GetOppositeDirection(dir);
	if (m_SnakeDirection == oppositeDir || changedDirection == true) return;
	changedDirection = true;
	m_SnakeDirection = dir;
}

void Snake::AddPiece() {
	m_Snake.push_back({ m_LastSnakePart.x,m_LastSnakePart.y, m_LastSnakePart.width, m_LastSnakePart.height });
}