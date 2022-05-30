#pragma once

#include <iostream>
#include <chrono>

#include <SDL/SDL_image.h>

#include <window/window.hpp>
#include <game/game.hpp>

namespace game::window {
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Surface* _screen;
	SDL_Joystick* _joystick;
	SDL_GameController* _controller;

	bool _running;

	int _rendered_frames;

	int _mouse_x;
	int _mouse_y;

	std::chrono::steady_clock::time_point _fps_old_time;
	std::chrono::steady_clock::time_point _frame_start_time;
	std::chrono::steady_clock::time_point _frame_end_time;
	std::chrono::milliseconds _total_frame_time{ 0 };

	float _delta_time{0};

	void init_fps();
	void init_window();
	void init_images();
	void init_input();

	void calculate_fps();
	void cap_fps();

	void handle_event(const SDL_Event& event);
	void handle_window_event(const SDL_Event& event);
	void handle_window_resize(const SDL_Event& event);
}


void game::window::init()
{
	_running = true;

	init_fps();
	init_window();
	init_images();
	init_input();
}

void game::window::init_fps()
{
	_fps_old_time = std::chrono::high_resolution_clock::now();
	_rendered_frames = 0;
}

void game::window::init_window()
{
	static constexpr auto initial_width = 800;
	static constexpr auto initial_height = 500;
	const auto initResult = SDL_Init(SDL_INIT_EVERYTHING);
	const auto createWindowResult = SDL_CreateWindowAndRenderer(initial_width, initial_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE, &_window, &_renderer);
}

void game::window::init_images()
{
	const auto imageFlags = IMG_INIT_PNG;
	const auto result = IMG_Init(imageFlags);
}

void game::window::init_input()
{
	_joystick = SDL_JoystickOpen(0);
	_controller = SDL_GameControllerOpen(0);
}

SDL_Surface* game::window::get_screen()
{
	if (_screen == nullptr) {
		_screen = SDL_GetWindowSurface(_window);
	}

	return _screen;
}

SDL_PixelFormat* game::window::get_pixel_format()
{
	return get_screen()->format;
}

SDL_Renderer* game::window::get_renderer()
{
	return _renderer;
}

SDL_Point game::window::get_mouse_position()
{
	return SDL_Point{
		.x = _mouse_x, 
		.y = _mouse_y
	};
}

SDL_Point game::window::get_renderer_size()
{
	int width;
	int height;

	SDL_GetRendererOutputSize(_renderer, &width, &height);

	return SDL_Point{
		.x = width,
		.y = height
	};
}

void game::window::capture_mouse()
{
	SDL_GetMouseState(&_mouse_x, &_mouse_y);
}

void game::window::poll_events()
{
	auto event = SDL_Event{};

	while (SDL_PollEvent(&event) != 0) {
		handle_event(event);
	}
}

void game::window::handle_event(const SDL_Event& event)
{
	if (event.type == SDL_QUIT) {
		_running = false;
		return;
	}

	if (event.type == SDL_WINDOWEVENT) {
		handle_window_event(event);
		return;
	}

	game::notify_event(event);
}

void game::window::handle_window_event(const SDL_Event& event)
{
	switch (event.window.event) {
	case SDL_WINDOWEVENT_RESIZED:
		handle_window_resize(event);
		break;
	}
}

void game::window::handle_window_resize(const SDL_Event& event)
{
	_screen = nullptr;
}

void game::window::clear_screen()
{
	_frame_start_time = std::chrono::high_resolution_clock::now();

	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(_renderer);
}

void game::window::draw_line(const glm::vec2& from, const glm::vec2& to)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
	SDL_RenderDrawLineF(_renderer, from.x, from.y, to.x, to.y);
}

void game::window::draw_vector(const glm::vec2& origin, const float magnitude)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
}

void game::window::draw_circle(const glm::vec2& origin, const float radius)
{
	auto x = radius - 1;
	auto y = 0.0f;
	auto dx = 1.0f;
	auto dy = 1.0f;
	auto error = dx - (static_cast<int>(radius) << 1);

	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
	
	while (x >= y) {
		SDL_RenderDrawPointF(_renderer, origin.x + x, origin.y + y);
		SDL_RenderDrawPointF(_renderer, origin.x + y, origin.y + x);
		SDL_RenderDrawPointF(_renderer, origin.x - y, origin.y + x);
		SDL_RenderDrawPointF(_renderer, origin.x - x, origin.y + y);
		SDL_RenderDrawPointF(_renderer, origin.x - x, origin.y - y);
		SDL_RenderDrawPointF(_renderer, origin.x - y, origin.y - x);
		SDL_RenderDrawPointF(_renderer, origin.x + y, origin.y - x);
		SDL_RenderDrawPointF(_renderer, origin.x + x, origin.y - y);

		if (error <= 0) {
			y++;
			error += dy;
			dy += 2;
		} 
		
		if (error > 0) {
			x--;
			dx += 2;
			error += dx - (static_cast<int>(radius) << 1);
		}
	}
}

void game::window::draw_texture(const glm::vec2& position, const glm::vec2& size, std::shared_ptr<game::Texture> texture)
{
	const auto rect = SDL_FRect{position.x, position.y, size.x, size.y};

	SDL_RenderCopyF(_renderer, texture->get_texture(), nullptr, &rect);
}

void game::window::draw_texture(const glm::vec2& origin, const float width, const float height, std::shared_ptr<game::Texture> texture)
{
	const auto rect = SDL_FRect{origin.x, origin.y, width, height};

	SDL_RenderCopyF(_renderer, texture->get_texture(), nullptr, &rect);
}

void game::window::draw_texture(const SDL_Rect* position, std::shared_ptr<game::Texture> texture)
{
	SDL_RenderCopy(_renderer, texture->get_texture(), nullptr, position);
}

void game::window::draw_texture(const SDL_FRect* position, std::shared_ptr<game::Texture> texture)
{
	SDL_RenderCopyF(_renderer, texture->get_texture(), nullptr, position);
}

const float game::window::get_delta_time() 
{
	return _delta_time;
}

void game::window::finish_drawing()
{
	SDL_RenderPresent(_renderer);

	_frame_end_time = std::chrono::high_resolution_clock::now();

	calculate_fps();
	cap_fps();
}

void game::window::calculate_fps()
{
	static constexpr auto second = std::chrono::seconds{1};

	const auto now = std::chrono::high_resolution_clock::now();
	const auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - _fps_old_time);

	_rendered_frames++;

	if (diff >= second) {
		const auto average_frame_time = _total_frame_time / _rendered_frames;

		std::cout << "fps: " << _rendered_frames << std::endl;
		std::cout << "avg frame time: " << average_frame_time << std::endl << std::endl;

		_fps_old_time = now;
		_rendered_frames = 0;
		_total_frame_time = std::chrono::milliseconds{0};
	}
}

void game::window::cap_fps()
{
	static constexpr auto fps = 144;
	static constexpr auto ms_per_frame = std::chrono::milliseconds{1000 / fps};

	const auto frame_time = std::chrono::duration_cast<std::chrono::milliseconds>(_frame_end_time - _frame_start_time);
	_total_frame_time += frame_time;

	if (frame_time < ms_per_frame) {
		const auto diff = ms_per_frame - frame_time;

		SDL_Delay(static_cast<Uint32>(diff.count()));
	}

	const auto now = std::chrono::high_resolution_clock::now();
	
	_delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - _frame_start_time).count();
}

bool game::window::is_running()
{
	return _running;
}

void game::window::close()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);

	IMG_Quit();
	SDL_Quit();
}