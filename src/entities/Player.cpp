#pragma once

#include <iostream>
#include <limits>
#include <valarray>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

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

	_left_joystick_x = 0.0f;
	_left_joystick_y = 0.0f;
	_right_joystick_x = 0.0f;
	_right_joystick_y = 0.0f;

	_left_joystick = glm::vec2{_left_joystick_x, _left_joystick_y};
	_right_joystick = glm::vec2{_right_joystick_x, _right_joystick_y};

	_running = false;
	_moving = false;

	_left_joystick_x_set = false;
	_left_joystick_y_set = false;

	_changing_direction = false;
	_deccelerating = false;
	_accelerating = false;

	_previous_directions = std::vector<glm::vec2>{};

	_last_directions_clear = std::chrono::high_resolution_clock::now();

	_texture = game::make_texture("images/player.png");

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

	static auto x_dir = 0;
	static auto y_dir = 0;

	_moving = true;

	if (axis == 0) {
		_left_joystick_x = value;
		_moving = true;

		_left_joystick_x_set = true;

		if (value < -deadzone) {
			x_dir = -1;
		} else if (value > deadzone) {
			x_dir = 1;
		} else {
			x_dir = 0;
		}
	} else if (axis == 1) {
		_left_joystick_y = value;
		_moving = true;

		_left_joystick_y_set = true;

		if (value < -deadzone) {
			y_dir = -1;
		} else if (value > deadzone) {
			y_dir = 1;
		} else {
			y_dir = 0;
		}
	}

	if (_moving && glm::abs(_left_joystick_x) < deadzone && glm::abs(_left_joystick_y) < deadzone) {
		_moving = false;
	}

	if (_moving) {
		_left_joystick = glm::vec2{ _left_joystick_x, _left_joystick_y };
	}

	if (x_dir != 0 || y_dir != 0) {
		const auto angle = glm::degrees(std::atan2(y_dir, x_dir));
		_angle = angle;
	}

	//_new_direction = _left_joystick;

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
	} else {
		game::camera::end_move();
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
	_previous_directions.emplace_back(_left_joystick);

	const auto& first_direction = _previous_directions.front();
	const auto& final_direction = _previous_directions.back();
	const auto direction_delta = glm::distance(first_direction, final_direction);

	if (!_changing_direction && direction_delta > 1.0f) {
		std::cout << "change direction: " << direction_delta << std::endl;
		_changing_direction = true;
		_deccelerating = true;
		_accelerating = false;
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
		if (_moving) {
			_direction = final_direction;

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

	static constexpr auto clear_diff = std::chrono::milliseconds{75};
	const auto now = std::chrono::high_resolution_clock::now();
	const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_directions_clear);

	if (diff >= clear_diff) {
		std::cout << _previous_directions.size() << std::endl;
		_previous_directions.clear();
		_last_directions_clear = now;
	}
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