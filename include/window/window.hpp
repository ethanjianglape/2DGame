#pragma once
#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <chrono>

#include <SDL/SDL.h>
#include <glm/vec2.hpp>

#include <texture/texture.hpp>

namespace game::window {
	void init();

	SDL_Surface* get_screen();
	SDL_PixelFormat* get_pixel_format();
	SDL_Renderer* get_renderer();

	SDL_Point get_mouse_position();
	SDL_Point get_renderer_size();

	const float get_delta_time();

	void poll_events();
	void capture_mouse();
	void clear_screen();

	void draw_line(const glm::vec2& from, const glm::vec2& to);
	void draw_vector(const glm::vec2& origin, const float magnitude);
	void draw_circle(const glm::vec2& origin, const float radius);
	void draw_texture(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<game::Texture> texture);
	void draw_texture(const glm::vec2& origin, const float width, const float height, const double angle, std::shared_ptr<game::Texture> texture);
	void draw_texture(const SDL_Rect* position, std::shared_ptr<game::Texture> texture);
	void draw_texture(const SDL_FRect* position, std::shared_ptr<game::Texture> texture);
	void finish_drawing();

	void close();

	bool is_running();
}

#endif