#pragma once

#include "Logger/Logger.h"

class TiledMap : public ILoggable {
private:
	unsigned int m_Width;
	unsigned int m_Height;
	bool m_DarkMode;

	Texture2D m_TileTexture;
public:
	TiledMap(const Texture2D& tileTexture, unsigned int width, unsigned int height);
	virtual ~TiledMap();

	void Render() const;

	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }
	inline unsigned int GetTileDimension() const { return m_TileTexture.width; }

	inline void ToggleDarkMode() { m_DarkMode = !m_DarkMode; }

	// Ereditato tramite ILoggable
	virtual std::string ToString() const override;
};
