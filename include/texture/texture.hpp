#pragma once
#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#include <memory>
#include <string>

#include <SDL/SDL.h>

namespace game {
	class Texture {
	private:
		SDL_Texture* _texture;
	public:
		Texture(std::string_view uri);

		SDL_Texture* get_texture() const noexcept;
	};

	std::shared_ptr<Texture> make_texture(std::string_view uri);
}

#endif