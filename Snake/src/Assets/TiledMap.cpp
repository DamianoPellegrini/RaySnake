#include "pch.h"
#include "TiledMap.h"

TiledMap::TiledMap(const Texture2D& tileTexture, unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height), m_TileTexture(tileTexture), m_DarkMode(false) {}

TiledMap::~TiledMap() {}

void TiledMap::Render() const {


	/*for (unsigned int i = 0; i < m_Width; i++) {
		for (unsigned int j = 0; j < m_Height; j++) {
			Color c;
			if (!m_DarkMode) {
				if ((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
					c = WHITE;
				else
					c = { 245,245,245,255 };
			} else {
				if ((j % 2 == 0 && i % 2 == 0) || (j % 2 != 0 && i % 2 != 0))
					c = { 80, 80, 80, 255 };
				else
					c = { 50, 50, 50, 255 };
			}
			DrawTextureV(m_TileTexture, { .0f + i * GetTileDimension(),.0f + j * GetTileDimension() }, c);
		}
	}*/
	DrawTextureQuad(m_TileTexture, { (float)m_Width, (float)m_Height }, { 8,8 }, { 0,0, (float)GetScreenWidth(), (float)GetScreenHeight() }, m_DarkMode ? Color({ 50, 50, 50, 255 }) : Color(WHITE));
}

std::string TiledMap::ToString() const {
	std::stringstream ss;
	ss << "Map:" << std::endl
		<< "\tWidth:\t" << m_Width << std::endl
		<< "\tHeight:\t" << m_Height << std::endl
		<< "\tTexture ID:\t" << m_TileTexture.id << std::endl;
	return ss.str();
}