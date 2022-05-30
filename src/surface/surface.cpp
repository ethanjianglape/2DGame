#pragma once

#include <iostream>
#include <SDL/SDL_image.h>

#include <surface/surface.hpp>

namespace game {
	Surface::Surface(SDL_PixelFormat* format, std::string_view uri) {
		SDL_Surface* surface = IMG_Load(uri.data());

		if (surface == nullptr) {
			std::cerr << "Surface failed to load" << std::endl;
			return;
		}

		SDL_Surface* optimizedSurface = SDL_ConvertSurface(surface, format, 0);

		if (optimizedSurface == nullptr) {
			std::cerr << "Surface failed to load" << std::endl;
			return;
		}

		SDL_FreeSurface(surface);

		_surface = optimizedSurface;
	}

	SDL_Surface* Surface::get_surface() const noexcept {
		return _surface;
	}
}
