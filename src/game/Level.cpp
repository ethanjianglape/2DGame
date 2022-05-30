#pragma once

#include <cstdlib>

#include <game/Level.hpp>
#include <game/Camera.hpp>

void game::Level::generate()
{
	int width = 100;
	int height = 100;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			const auto world_position = glm::vec2{x, y};
			const auto r = rand() % 50;

			if (r < 40) {
				_tiles.push_back(game::make_grass_tile(world_position));
			} else {
				_tiles.push_back(game::make_rock_tile(world_position));
			}
		}
	}

	game::camera::init();

	_player = std::make_unique<game::Player>();
}

void game::Level::handle_keypress(const SDL_Event& event)
{
	_player->handle_keypress(event);
}

void game::Level::handle_joystick(const SDL_Event& event)
{
	_player->handle_joystick(event);
}

void game::Level::update()
{
	_player->update();
	game::camera::update();
}

void game::Level::draw() const noexcept
{
	const auto& draw_offset = game::camera::get_render_offset();

	for (auto&& tile : _tiles) {
		tile->draw(draw_offset);
	}

	_player->draw();

	game::camera::draw();
}