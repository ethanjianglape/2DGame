#pragma once

#include <iostream>
#include <limits>
#include <valarray>
#include <vector>
#include <chrono>

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include <game/Gamepad.hpp>

namespace game::pad {
	glm::vec2 _left_joystick_direction;
	glm::vec2 _right_joystick_direction;

	glm::vec2 _initial_left_joystick_direction;
	glm::vec2 _initial_right_joystick_direction;

	//std::vector<glm::vec2> _left_joystick_directions;
	//std::vector<glm::vec2> _right_joystick_directions;

	float _left_joystick_angle;
	float _left_joystick_angle_delta;

	float _initial_left_joystick_angle;
	float _initial_right_joystick_angle;

	float _right_joystick_angle;
	float _right_joystick_angle_delta;

	std::chrono::steady_clock::time_point _previous_joystick_directions_clear;

	constexpr glm::vec2 JOYSTICK_ORIGIN = glm::vec2{ 0.0f, 0.0f };

	constexpr std::chrono::milliseconds JOYSTICK_CLEAR_INTERVAL = std::chrono::milliseconds{75};

	constexpr int LEFT_JOYSTICK_X_AXIS = 0;
	constexpr int LEFT_JOYSTICK_Y_AXIS = 1;
	constexpr int RIGHT_JOYSTICK_X_AXIS = 2;
	constexpr int RIGHT_JOYSTICK_Y_AXIS = 3;

	constexpr float JOYSTICK_DIRECTION_LIMIT = static_cast<float>(std::numeric_limits<Sint16>::max());
	constexpr float JOYSTICK_DEADZONE = 8000.0f / JOYSTICK_DIRECTION_LIMIT;

	constexpr float PI = glm::pi<float>();

	void handle_joystick(const SDL_Event& event);
	void set_joystick_angles();

	float calculate_angle(const glm::vec2& jostick_direction);
}

void game::pad::init() 
{
	_left_joystick_direction = JOYSTICK_ORIGIN;
	_right_joystick_direction = JOYSTICK_ORIGIN;

	_initial_left_joystick_direction = JOYSTICK_ORIGIN;
	_initial_right_joystick_direction = JOYSTICK_ORIGIN;

	_left_joystick_angle = 0.0f;
	_left_joystick_angle_delta = 0.0f;
	_right_joystick_angle = 0.0f;
	_right_joystick_angle_delta = 0.0f;

	_initial_left_joystick_angle = 0.0f;
	_initial_right_joystick_angle = 0.0f;

	_previous_joystick_directions_clear = std::chrono::steady_clock::now();
}

const glm::vec2& game::pad::get_joystick_direction(Joystick joystick)
{
	switch (joystick) {
	case Joystick::LEFT:
		return _left_joystick_direction;
	case Joystick::RIGHT:
		return _right_joystick_direction;
	}
}

float game::pad::get_joystick_angle(Joystick joystick)
{
	switch (joystick) {
	case Joystick::LEFT:
		return _left_joystick_angle;
	case Joystick::RIGHT:
		return _right_joystick_angle;
	}
}

float game::pad::get_joystick_angle_delta(Joystick joystick)
{
	switch (joystick) {
	case Joystick::LEFT:
		return _left_joystick_angle_delta;
	case Joystick::RIGHT:
		return _right_joystick_angle_delta;
	}
}

bool game::pad::is_joystick_in_deadzone(Joystick joystick) 
{
	const auto& joystick_direction = get_joystick_direction(joystick);

	return joystick_direction.x < JOYSTICK_DEADZONE && joystick_direction.y < JOYSTICK_DEADZONE;
}

void game::pad::handle_event(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_JOYAXISMOTION:
		handle_joystick(event);
		break;
	}
}

void game::pad::handle_joystick(const SDL_Event& event)
{
	const auto direction = static_cast<float>(event.jaxis.value) / JOYSTICK_DIRECTION_LIMIT;

	switch (event.jaxis.axis) {
	case LEFT_JOYSTICK_X_AXIS:
		_left_joystick_direction.x = direction;
		break;
	case LEFT_JOYSTICK_Y_AXIS:
		_left_joystick_direction.y = direction;
		break;
	case RIGHT_JOYSTICK_X_AXIS:
		_right_joystick_direction.x = direction;
		break;
	case RIGHT_JOYSTICK_Y_AXIS:
		_right_joystick_direction.y = direction;
		break;
	}
}

void game::pad::update()
{
	set_joystick_angles();

	const auto now = std::chrono::steady_clock::now();
	const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - _previous_joystick_directions_clear);

	if (diff >= JOYSTICK_CLEAR_INTERVAL) {
		_initial_left_joystick_direction = _left_joystick_direction;
		_initial_left_joystick_angle = _left_joystick_angle;

		_initial_right_joystick_direction = _right_joystick_direction;
		_initial_right_joystick_angle = _right_joystick_angle;

		_previous_joystick_directions_clear = now;
	}
}

void game::pad::set_joystick_angles()
{
	_left_joystick_angle = calculate_angle(_left_joystick_direction);
	_right_joystick_angle = calculate_angle(_right_joystick_direction);

	_left_joystick_angle_delta = glm::abs(_left_joystick_angle - _initial_left_joystick_angle);
	_right_joystick_angle_delta = glm::abs(_right_joystick_angle - _initial_right_joystick_angle);
}

float game::pad::calculate_angle(const glm::vec2& joystick_direction)
{
	const auto theta = std::atan2(joystick_direction.y, joystick_direction.x);
	const auto theta_corrected = theta < 0 ? glm::abs(theta) : (2.0f * game::pad::PI) - theta;
	const auto degrees = glm::degrees(theta_corrected);

	return degrees;
}