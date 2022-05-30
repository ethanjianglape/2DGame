#pragma once
#ifndef _TEXTURE_CPP
#define _TEXTURE_CPP

#include <map>

#include <SDL/SDL_image.h>

#include <window/window.hpp>
#include <texture/texture.hpp>

std::map<std::string_view, std::shared_ptr<game::Texture>> _saved_textures;

game::Texture::Texture(std::string_view uri)
{
	SDL_Texture* texture = IMG_LoadTexture(game::window::get_renderer(), uri.data());

	_texture = texture;
}

SDL_Texture* game::Texture::get_texture() const noexcept
{
	return _texture;
}

std::shared_ptr<game::Texture> game::make_texture(std::string_view uri)
{
	const auto iterator = _saved_textures.find(uri);

	if (iterator != _saved_textures.end()) {
		return iterator->second;
	}

	auto texture = std::make_shared<game::Texture>(uri);

	_saved_textures.try_emplace(uri, texture);

	return texture;
}

#endif