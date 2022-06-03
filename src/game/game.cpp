#pragma once
#ifndef _GAME_CPP
#define _GAME_CPP

#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>

#include <glm/vec2.hpp>

#include <game/game.hpp>
#include <entities/Player.hpp>
#include <game/Level.hpp>
#include <game/Gamepad.hpp>

namespace game {
	std::unique_ptr<game::Level> _level;
}

void game::init()
{
	game::window::init();

	_level = std::make_unique<game::Level>();
	_level->generate();
}

void game::loop()
{
	while (game::window::is_running()) {
		game::window::poll_events();
		game::window::capture_mouse();

		game::pad::update();

		_level->update();

		game::window::clear_screen();

		_level->draw();

		game::window::finish_drawing();
	}

	game::window::close();
}

#endif
