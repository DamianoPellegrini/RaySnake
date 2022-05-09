#pragma once

#include "Assets/Game.h"

class Loader {
public:
	void Load(Game& game);

	void Unload(Game& game) const;

	static Loader* Get();
private:
	static Loader* s_Instance;

	Loader();
};
