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

	game::pad::add_button_action_handler(game::pad::ButtonActionHandler{
		.action = game::pad::ButtonAction::Pressed,
		.handler = [this] (const game::pad::ButtonState& state) { this->button_pressed(state); }
	});

	game::pad::add_button_action_handler(game::pad::ButtonActionHandler{
		.action = game::pad::ButtonAction::Held,
		.handler = [this](const game::pad::ButtonState& state) { this->button_held(state); }
	});

	game::pad::add_button_action_handler(game::pad::ButtonActionHandler{
		.action = game::pad::ButtonAction::Released,
		.handler = [this](const game::pad::ButtonState& state) { this->button_released(state); }
	});
}

void game::Player::init()
{
}

void game::Player::button_pressed(const game::pad::ButtonState& state)
{
	
}

void game::Player::button_held(const game::pad::ButtonState& state)
{
	const auto button = state.button;
	
	if (button == game::pad::Button::Circle) {
		_movement_state = MovementState::Running;
	}
}

void game::Player::button_released(const game::pad::ButtonState& state)
{
	const auto button = state.button;

	if (button == game::pad::Button::Circle) {
		_movement_state = MovementState::Walking;
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

	const auto& direction = game::pad::get_joystick_direction(game::pad::Joystick::Left);
	const auto angle = game::pad::get_joystick_angle(game::pad::Joystick::Left);
	const auto theta_delta = game::pad::get_joystick_angle_delta(game::pad::Joystick::Left);

	if (!_changing_direction && theta_delta >= 30.0f) {
		//std::cout << "change direction: " << theta_delta << std::endl;
		//_changing_direction = true;
		//_deccelerating = true;
		//_accelerating = false;
	}

	if (_state == PlayerState::Moving && game::pad::is_joystick_in_deadzone(game::pad::Joystick::Left)) {
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

	game::window::draw_texture(position, _width, _height, _angle, _texture);
}