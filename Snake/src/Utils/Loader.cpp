#include "pch.h"
#include "Loader.h"

//######################################
//	Singleton stuff
//######################################

Loader* Loader::s_Instance = nullptr;

Loader::Loader() {
	Loader::s_Instance = this;
}

Loader* Loader::Get() {
	if (s_Instance == nullptr)
		return new Loader();

	return s_Instance;
}

//######################################
//	Loader part
//######################################

void Loader::Load(Game& game) {
	int count = 0;
	//Textures
	{
		SLOG_TRACE("==================================");
		SLOG_TRACE("         Loading textures         ");
		SLOG_TRACE("==================================");
		std::string texturesDir = "resources/textures/";
		char** texturesFileName = GetDirectoryFiles(texturesDir.c_str(), &count);

		for (int i = 2; i < count; i++) {
			std::string texturePath = texturesDir + texturesFileName[i];
			game.m_Textures.emplace(GetFileNameWithoutExt(texturePath.c_str()), LoadTexture(texturePath.c_str()));
			SLOG_TRACE("[Texture] " + texturePath + " loaded!");
		}

		ClearDirectoryFiles();
	}

	//Fonts
	{
		SLOG_TRACE("==================================");
		SLOG_TRACE("          Loading fonts           ");
		SLOG_TRACE("==================================");
		std::string fontsDir = "resources/fonts/";
		char** fontsFileName = GetDirectoryFiles(fontsDir.c_str(), &count);

		for (int i = 2; i < count; i++) {
			std::string fontPath = fontsDir + fontsFileName[i];
			game.m_Fonts.emplace(GetFileNameWithoutExt(fontPath.c_str()), LoadFont(fontPath.c_str()));
			SLOG_TRACE("[Font] " + fontPath + " loaded!");
		}

		ClearDirectoryFiles();
	}
}

void Loader::Unload(Game& game) const {
	for (auto const& [key, val] : game.m_Textures) {
		UnloadTexture(val);
	}
	for (auto const& [key, val] : game.m_Fonts) {
		UnloadFont(val);
	}
}