#pragma once

#include <iostream>
#include <limits>

#include <glm/geometric.hpp>

#include <entities/Player.hpp>
#include <window/window.hpp>
#include <game/Camera.hpp>

namespace game {
	static constexpr auto left_axis_x = 0;
	static constexpr auto left_axis_y = 1;
	static constexpr auto right_axis_x = 2;
	static constexpr auto right_axis_y = 3;
}

game::Player::Player()
{
	_origin = glm::vec2{0, 0};
	_direction = glm::vec2{0, 0};

	_world_position = glm::vec2{0, 0};

	_width = 50;
	_height = 50;

	_acceleration = 20.0f;
	_decceleration = 40.0f;
	_walking_speed = 200.0f;
	_running_speed = 400.0f;
	_speed = 0.0f;

	_left_joystick_x = 0.0f;
	_left_joystick_y = 0.0f;
	_right_joystick_x = 0.0f;
	_right_joystick_y = 0.0f;

	_running = false;
	_moving = false;

	_texture = game::make_texture("images/green-sun.png");

	game::camera::center_on_position(get_world_position_center());
}

void game::Player::init()
{
}

void game::Player::handle_joystick(const SDL_Event& event)
{
	if (event.type == SDL_JOYAXISMOTION) {


		const auto axis = event.jaxis.axis;

		switch (axis) {
		case left_axis_x:
		case left_axis_y:
			handle_left_joystick(event);
			break;
		case right_axis_x:
		case right_axis_y:
			handle_right_joystick(event);
			break;
		}
	} else if (event.type == SDL_JOYBUTTONDOWN) {
		handle_button_down(event);
	} else if (event.type == SDL_JOYBUTTONUP) {
		handle_button_up(event);
	}
}

void game::Player::handle_left_joystick(const SDL_Event& event)
{
	static constexpr auto limit = static_cast<float>(std::numeric_limits<Sint16>::max());
	static constexpr auto deadzone = 8000.0f / limit;
	const auto value = static_cast<float>(event.jaxis.value) / limit;
	const auto axis = event.jaxis.axis;

	if (axis == 0) {
		_left_joystick_x = value;
		_moving = true;
	} else if (axis == 1) {
		_left_joystick_y = value;
		_moving = true;
	}

	_left_joystick = glm::vec2{_left_joystick_x, _left_joystick_y};
	_direction = _left_joystick;

	if (glm::abs(_left_joystick.x) < deadzone && glm::abs(_left_joystick.y) < deadzone) {
		_moving = false;
	}
}

void game::Player::handle_right_joystick(const SDL_Event& event)
{
	static constexpr auto limit = static_cast<float>(std::numeric_limits<Sint16>::max());
	static constexpr auto deadzone = 8000.0f / limit;
	const auto value = static_cast<float>(event.jaxis.value) / limit;
	const auto axis = event.jaxis.axis;

	if (axis == right_axis_x) {
		_right_joystick_x = value;
	} else if (axis == right_axis_y) {
		_right_joystick_y = value;
	}

	_right_joystick = glm::vec2{_right_joystick_x, _right_joystick_y};

	if (glm::abs(_right_joystick.x) > deadzone || glm::abs(_right_joystick.y) > deadzone) {
		game::camera::begin_move(_right_joystick);
	}
}


void game::Player::handle_button_down(const SDL_Event& event) 
{
	if (event.jbutton.button == 1) {
		_running = true;
	}
}

void game::Player::handle_button_up(const SDL_Event& event)
{
	if (event.jbutton.button == 1) {
		_running = false;
	}
}

void game::Player::handle_keypress(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		handle_key_down(event);
		break;
	case SDL_KEYUP:
		handle_key_up(event);
		break;
	}
}

void game::Player::handle_key_up(const SDL_Event& event)
{
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		break;
	case SDLK_DOWN:
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_SPACE:
		break;
	}
}

void game::Player::handle_key_down(const SDL_Event& event)
{
	switch (event.key.keysym.sym) {
	case SDLK_UP:
		break;
	case SDLK_DOWN:
		break;
	case SDLK_LEFT:
		break;
	case SDLK_RIGHT:
		break;
	case SDLK_SPACE:
		break;
	}
}

const float game::Player::get_max_speed() const noexcept
{
	if (_running) {
		return _running_speed;
	}

	return _walking_speed;
}

void game::Player::update()
{
	const auto max_speed = get_max_speed();

	if (_moving) {
		if (_speed < max_speed) {
			_speed += _acceleration;
		} else if (_speed > max_speed) {
			_speed -= _decceleration;
		}
	} else {
		if (_speed < _decceleration) {
			_speed = 0.0f;
		} else if (_speed > 0) {
			_speed -= _decceleration;
		}
	}

	_world_position += get_velocity();

	game::camera::follow_position(get_world_position_center());
}

const glm::vec2 game::Player::get_velocity() const noexcept 
{
	return _direction * _speed * game::window::get_delta_time();
}

const glm::vec2& game::Player::get_world_position() const noexcept
{
	return _world_position;
}

const glm::vec2 game::Player::get_world_position_center() const noexcept
{
	return glm::vec2{_world_position.x + (_width / 2), _world_position.y + (_height / 2)};
}

void game::Player::draw() const noexcept
{
	const auto position = game::camera::get_relative_position(_world_position);

	game::window::draw_texture(position, _width, _height, _texture);
}