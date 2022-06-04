#pragma once

#include <iostream>
#include <limits>
#include <valarray>
#include <vector>
#include <chrono>
#include <map>

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include <game/Gamepad.hpp>

namespace game::pad {
	SDL_Joystick* _joystick;
	SDL_GameController* _controller;

	glm::vec2 _left_joystick_direction;
	glm::vec2 _right_joystick_direction;

	glm::vec2 _initial_left_joystick_direction;
	glm::vec2 _initial_right_joystick_direction;

	std::map<Button, ButtonState> _button_state;

	std::vector<ButtonActionHandler> _button_action_handlers;

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

	void handle_button_down(const SDL_Event& event);
	void handle_button_up(const SDL_Event& event);
	void handle_joystick(const SDL_Event& event);
	void set_joystick_angles();
	void set_initial_joysticks();

	void trigger_button_action_handlers();
	void update_button_press_durations();

	float calculate_angle(const glm::vec2& jostick_direction);
}

void game::pad::init() 
{
	_joystick = SDL_JoystickOpen(0);
	_controller = SDL_GameControllerOpen(0);

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

	_button_state = std::map<Button, ButtonState>{
		{Button::Cross, ButtonState{Button::Cross, ButtonAction::None}},
		{Button::Circle, ButtonState{Button::Circle, ButtonAction::None}},
		{Button::Square, ButtonState{Button::Square, ButtonAction::None}},
		{Button::Triangle, ButtonState{Button::Triangle, ButtonAction::None}},
		{Button::L3, ButtonState{Button::L3, ButtonAction::None}},
		{Button::R3, ButtonState{Button::R3, ButtonAction::None}},
		{Button::L1, ButtonState{Button::L1, ButtonAction::None}},
		{Button::R1, ButtonState{Button::R1, ButtonAction::None}},
		{Button::Up, ButtonState{Button::Up, ButtonAction::None}},
		{Button::Down, ButtonState{Button::Down, ButtonAction::None}},
		{Button::Left, ButtonState{Button::Left, ButtonAction::None}},
		{Button::Right, ButtonState{Button::Right, ButtonAction::None}}
	};

	_previous_joystick_directions_clear = std::chrono::steady_clock::now();
}

void game::pad::add_button_action_handler(ButtonActionHandler handler)
{
	_button_action_handlers.push_back(handler);
}

const glm::vec2& game::pad::get_joystick_direction(Joystick joystick)
{
	switch (joystick) {
	case Joystick::Left:
		return _left_joystick_direction;
	case Joystick::Right:
		return _right_joystick_direction;
	}
}

float game::pad::get_joystick_angle(Joystick joystick)
{
	switch (joystick) {
	case Joystick::Left:
		return _left_joystick_angle;
	case Joystick::Right:
		return _right_joystick_angle;
	}
}

float game::pad::get_joystick_angle_delta(Joystick joystick)
{
	switch (joystick) {
	case Joystick::Left:
		return _left_joystick_angle_delta;
	case Joystick::Right:
		return _right_joystick_angle_delta;
	}
}

bool game::pad::is_joystick_in_deadzone(Joystick joystick) 
{
	const auto& joystick_direction = get_joystick_direction(joystick);

	return glm::abs(joystick_direction.x) < JOYSTICK_DEADZONE &&
		   glm::abs(joystick_direction.y) < JOYSTICK_DEADZONE;
}

void game::pad::handle_event(const SDL_Event& event)
{
	switch (event.type) {
	case SDL_JOYAXISMOTION:
		handle_joystick(event);
		break;
	case SDL_JOYBUTTONDOWN:
		handle_button_down(event);
		break;
	case SDL_JOYBUTTONUP:
		handle_button_up(event);
		break;
	}
}

void game::pad::handle_button_down(const SDL_Event& event)
{
	const auto button = static_cast<Button>(event.jbutton.button);
	auto& state = _button_state[button];

	state.action = ButtonAction::Pressed;
	state.press_start_time = std::chrono::steady_clock::now();
	state.held_duration = std::chrono::milliseconds{0};
}

void game::pad::handle_button_up(const SDL_Event& event)
{
	const auto button = static_cast<Button>(event.jbutton.button);
	auto& state = _button_state[button];

	state.action = ButtonAction::Released;
}

void game::pad::handle_joystick(const SDL_Event& event)
{
	const auto value = static_cast<float>(event.jaxis.value) / JOYSTICK_DIRECTION_LIMIT;

	switch (event.jaxis.axis) {
	case LEFT_JOYSTICK_X_AXIS:
		_left_joystick_direction.x = value;
		break;
	case LEFT_JOYSTICK_Y_AXIS:
		_left_joystick_direction.y = value;
		break;
	case RIGHT_JOYSTICK_X_AXIS:
		_right_joystick_direction.x = value;
		break;
	case RIGHT_JOYSTICK_Y_AXIS:
		_right_joystick_direction.y = value;
		break;
	}
}

void game::pad::update()
{
	set_joystick_angles();
	trigger_button_action_handlers();
	update_button_press_durations();
	set_initial_joysticks();
}

void game::pad::set_joystick_angles()
{
	_left_joystick_angle = calculate_angle(_left_joystick_direction);
	_right_joystick_angle = calculate_angle(_right_joystick_direction);

	const auto left_joystick_angle_delta1 = glm::abs(_left_joystick_angle - _initial_left_joystick_angle);
	const auto left_joystick_angle_delta2 = glm::abs(_initial_left_joystick_angle - _left_joystick_angle);

	_left_joystick_angle_delta = glm::min(left_joystick_angle_delta1, left_joystick_angle_delta2);
	_right_joystick_angle_delta = glm::abs(_right_joystick_angle - _initial_right_joystick_angle);
}

void game::pad::trigger_button_action_handlers()
{
	for (const auto& [button, state] : _button_state) {
		if (state.action == ButtonAction::None) {
			continue;
		}

		for (const auto& handler : _button_action_handlers) {
			if (handler.action == state.action) {
				handler.handler(state);
			}
		}
	}
}

void game::pad::update_button_press_durations()
{
	const auto now = std::chrono::steady_clock::now();

	for (auto&& [button, state] : _button_state) {
		if (state.action == ButtonAction::Pressed) {
			state.action = ButtonAction::Held;
		}

		if (state.action == ButtonAction::Held) {
			state.held_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - state.press_start_time);
		}

		if (state.action == ButtonAction::Released) {
			state.action = ButtonAction::None;
		}
	}
}

void game::pad::set_initial_joysticks()
{
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

float game::pad::calculate_angle(const glm::vec2& joystick_direction)
{
	const auto theta = std::atan2(joystick_direction.y, joystick_direction.x);
	const auto theta_corrected = theta < 0 ? glm::abs(theta) : (2.0f * game::pad::PI) - theta;
	const auto degrees = glm::degrees(theta_corrected);

	return degrees;
}