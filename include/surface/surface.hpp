#pragma once
#ifndef _SURFACE_HPP_
#define _SURFACE_HPP_

#include <string>
#include <SDL/SDL.h>

#include <window/window.hpp>

namespace game {
	class Surface {
	private:
		SDL_Surface* _surface;
	public:
		Surface(SDL_PixelFormat* format, std::string_view uri);

		SDL_Surface* get_surface() const noexcept;
	};
}

#endif