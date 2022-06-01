#pragma once

#include <iostream>

#include <game/Camera.hpp>
#include <window/window.hpp>

#include <glm/geometric.hpp>

namespace game::camera {
	glm::vec2 _position;
	glm::vec2 _position_inverse;
	glm::vec2 _size;
	glm::vec2 _velocity;
	glm::vec2 _offset;
	glm::vec2 _offset_direction;

	float _offset_speed;
	float _offset_return_speed;
	float _offset_max;

	bool _off_center;
	bool _moving;
	bool _returning;

	const float get_offset_speed();
}

void game::camera::init()
{
	const auto window_size = game::window::get_renderer_size();

	_off_center = false;
	_moving = false;
	_returning = false;

	_offset_speed = 200.0f;
	_offset_return_speed = 400.0f;
	_offset_max = 50.0f;

	_offset = glm::vec2{0, 0};
	_offset_direction = glm::vec2{0, 0};
	_velocity = glm::vec2{0, 0};
	_position = glm::vec2{0, 0};
	_position_inverse = _position * -1.0f;
	_size = glm::vec2{window_size.x, window_size.y};
}

void game::camera::begin_move(const glm::vec2& direction)
{
	_offset_direction = direction;
	_moving = true;
}

void game::camera::end_move()
{
	_moving = false;
}

void game::camera::move_offset_in_direction(const glm::vec2& direction)
{
	_offset_direction = direction;
	_moving = true;
	_returning = false;
}

void game::camera::move_offset_to_center()
{
	if (!_off_center) {
		return;
	}

	const auto from = get_offset_world_position();
	const auto to = get_center_world_position();
	const auto diff = to - from;
	const auto length = glm::length(diff);

	if (length > 0) {
		_offset_direction = diff / length;
		_moving = false;
		_returning = true;
	}
}

void game::camera::center_on_position(const glm::vec2& position)
{
	_position = glm::vec2{position.x - (_size.x / 2), position.y - (_size.y / 2)};
	_position_inverse = _position * -1.0f;
}

void game::camera::follow_position(const glm::vec2& position)
{
	static constexpr auto lerp = 5.0f;

	const auto center = get_center_world_position();
	const auto v = (position - center) * lerp * game::window::get_delta_time();

	_velocity = v;
}

const float game::camera::get_offset_speed()
{
	if (_returning) {
		return _offset_return_speed;
	}

	return _offset_speed;
}

void game::camera::update()
{
	if (_moving) {
		_velocity = _offset_direction * _offset_speed * game::window::get_delta_time();
	}

	_position += _velocity;
	_position_inverse = _position * -1.0f;
}

void game::camera::draw()
{
	const auto x = _size.x / 2;
	const auto y = _size.y / 2;
	const auto u = glm::vec2{x, y - 10};
	const auto d = glm::vec2{x, y + 10};
	const auto l = glm::vec2{x - 10, y};
	const auto r = glm::vec2{x + 10, y};

	game::window::draw_line(u, d);
	game::window::draw_line(l, r);
}

glm::vec2 game::camera::get_relative_position(const glm::vec2& entity_position)
{
	return entity_position + _position_inverse;
}

glm::vec2 game::camera::get_center_world_position()
{
	return glm::vec2{_position.x + (_size.x / 2), _position.y + (_size.y / 2)};
}

glm::vec2 game::camera::get_offset_world_position()
{
	return get_center_world_position();
}

glm::vec2 game::camera::get_render_offset()
{
	return _position_inverse;
}