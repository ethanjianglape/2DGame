#pragma once

#include <SDL/SDL.h>

namespace game {
	class IControlable {
		virtual void handle_keypress(const SDL_Event& event) = 0;
	};
}