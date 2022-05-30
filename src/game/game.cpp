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

namespace game {
	std::unique_ptr<game::Level> _level;

	void handle_keypress(const SDL_Event& event);
	void handle_joystick(const SDL_Event& event);
}

void game::init()
{
	game::window::init();

	_level = std::make_unique<game::Level>();
	_level->generate();
}

void game::notify_event(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
		game::handle_keypress(event);
		return;
	}

	if (event.type == SDL_JOYAXISMOTION || event.type == SDL_JOYBUTTONDOWN || event.type == SDL_JOYBUTTONUP) {
		game::handle_joystick(event);
		return;
	}
}

void game::handle_keypress(const SDL_Event& event)
{
	_level->handle_keypress(event);
}

void game::handle_joystick(const SDL_Event& event)
{
	_level->handle_joystick(event);
}

void game::loop()
{
	while (game::window::is_running()) {
		game::window::poll_events();
		game::window::capture_mouse();

		_level->update();

		game::window::clear_screen();

		_level->draw();

		game::window::finish_drawing();
	}

	game::window::close();
}

#endif
