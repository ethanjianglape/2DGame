#pragma once

#include <game/Camera.hpp>
#include <window/window.hpp>

namespace game::camera {
	glm::vec2 _position;
	glm::vec2 _position_inverse;
	glm::vec2 _size;

}

void game::camera::init()
{
	const auto window_size = game::window::get_renderer_size();

	_position = glm::vec2{0, 0};
	_position_inverse = _position * -1.0f;
	_size = glm::vec2{window_size.x, window_size.y};
}

void game::camera::move_center_position(const glm::vec2& direction)
{

}

void game::camera::center_on_position(const glm::vec2& position)
{

}

void game::camera::follow_position(const glm::vec2& world_position)
{

}

void game::camera::update()
{

}

void game::camera::draw()
{

}

glm::vec2 game::camera::get_relative_position(const glm::vec2& entity_position)
{
	return entity_position + _position_inverse;
}

glm::vec2 game::camera::get_world_center_position()
{
	return glm::vec2{_position.x + (_size.x / 2), _position.y + (_size.y / 2)};
}

glm::vec2 game::camera::get_render_offset()
{
	return _position_inverse;
}

const glm::vec2& game::camera::get_position()
{
	return _position;
}

const glm::vec2& game::camera::get_size()
{
	return _size;
}