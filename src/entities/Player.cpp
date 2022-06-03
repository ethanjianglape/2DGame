#pragma once

#include <iostream>
#include <limits>
#include <valarray>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include <entities/Player.hpp>
#include <window/window.hpp>
#include <game/Camera.hpp>
#include <game/Gamepad.hpp>

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
	_previous_direction = _direction;

	_world_position = glm::vec2{0, 0};

	_width = 50;
	_height = 50;

	_acceleration = 500.0f;
	_decceleration = 700.0f;
	_direction_acceleration = 800.0f;
	_direction_decceleration = 4000.0f;
	_walking_speed = 200.0f;
	_running_speed = 400.0f;
	_speed = 0.0f;

	_angle = 0.0f;

	_state = PlayerState::Idle;
	_movement_state = MovementState::Walking;

	_changing_direction = false;
	_deccelerating = false;
	_accelerating = false;

	_texture = game::make_texture("images/player.png");

	game::camera::center_on_position(get_world_position_center());
}

void game::Player::init()
{
}

void game::Player::handle_joystick(const SDL_Event& event)
{
	if (event.type == SDL_JOYAXISMOTION) {
	} else if (event.type == SDL_JOYBUTTONDOWN) {
		handle_button_down(event);
	} else if (event.type == SDL_JOYBUTTONUP) {
		handle_button_up(event);
	}
}

void game::Player::handle_button_down(const SDL_Event& event) 
{
	if (event.jbutton.button == 1) {
		_movement_state = MovementState::Running;
	}
}

void game::Player::handle_button_up(const SDL_Event& event)
{
	if (event.jbutton.button == 1) {
		_movement_state = MovementState::Walking;
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
	if (_movement_state == MovementState::Running) {
		return _running_speed;
	}

	return _walking_speed;
}

void game::Player::update()
{
	const auto& direction = game::pad::get_joystick_direction(game::pad::Joystick::LEFT);
	const auto theta_delta = game::pad::get_joystick_angle_delta(game::pad::Joystick::LEFT);

	//std::cout << theta_delta << "\n";

	if (!_changing_direction && theta_delta >= 30.0f) {
		//std::cout << "change direction: " << theta_delta << std::endl;
		//_changing_direction = true;
		//_deccelerating = true;
		//_accelerating = false;
	}

	if (_state == PlayerState::Moving && game::pad::is_joystick_in_deadzone(game::pad::Joystick::LEFT)) {
		_state = PlayerState::Idle;
	} else {
		_state = PlayerState::Moving;
	}

	const auto max_speed = get_max_speed();
	const auto delta_time = game::window::get_delta_time();

	if (_changing_direction) {
		if (_deccelerating) {
			_speed -= _direction_decceleration * delta_time;

			if (_speed <= 0) {
				_speed = 0.0f;
				_deccelerating = false;
			}
		} else if (_accelerating) {
			_speed += _direction_acceleration * delta_time;

			if (_speed >= max_speed) {
				_speed = max_speed;
				_accelerating = false;
			}
		}

		if (!_deccelerating && !_accelerating) {
			_changing_direction = false;
		}
	}

	if (!_changing_direction) {
		if (_state == PlayerState::Moving) {
			_direction = direction;

			if (_speed < max_speed) {
				_speed += _acceleration * delta_time;
			} else if (_speed > max_speed) {
				_speed -= _decceleration * delta_time;
			}
		} else {
			if (_speed < 0) {
				_speed = 0.0f;
			} else if (_speed > 0) {
				_speed -= _decceleration * delta_time;
			}
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

	game::window::draw_texture(position, _width, _height, _angle + 90, _texture);
}