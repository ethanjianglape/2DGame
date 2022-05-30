#pragma once

#include <memory>
#include <iostream>
#include <cstdlib>

#include <entities/BouncingBall.hpp>
#include <texture/texture.hpp>
#include <window/window.hpp>

#include <glm/geometric.hpp>

game::BouncingBall::BouncingBall()
{
	_direction = glm::vec2{1.0f, 1.0f};

	const auto size = (rand() % 30) + 20;

	_width = size;
	_height = size;
	mass = size / 10;

	_texture = game::make_texture("images/cool-sun.png");
}

game::BouncingBall::BouncingBall(glm::vec2 origin) : BouncingBall()
{
	_origin = origin;
}

bool game::BouncingBall::collides_with(const BouncingBall& other)
{
	const auto my_hitbox = get_hitbox();
	const auto other_hitbox = other.get_hitbox();

	if (my_hitbox.position == other_hitbox.position) {
		return false;
	}

	const auto x1 = my_hitbox.position.x;
	const auto x2 = other_hitbox.position.x;

	const auto y1 = my_hitbox.position.y;
	const auto y2 = other_hitbox.position.y;

	const auto r1 = my_hitbox.radius;
	const auto r2 = other_hitbox.radius;

	return glm::pow(x2 - x1, 2) + glm::pow(y2 - y1, 2) <= glm::pow(r1 + r2, 2);
}

void game::BouncingBall::collision_check(BouncingBall& other)
{
	if (collides_with(other)) {
		collide(other);
	}
}

void game::BouncingBall::set_velocity(glm::vec2 velocity)
{
	_direction = velocity;
}

const glm::vec2& game::BouncingBall::get_velocity() const
{
	return _direction;
}

const glm::vec2& game::BouncingBall::get_position() const
{
	return _origin;
}

const glm::vec2 game::BouncingBall::get_size() const
{
	return glm::vec2{_width, _height};
}


const game::Hitbox game::BouncingBall::get_hitbox() const
{
	const auto center_x = _origin.x + (_width / 2);
	const auto center_y = _origin.y + (_height / 2);

	return Hitbox{
		.position = glm::vec2{center_x, center_y},
		.radius = (_width / 2)
	};
}

void game::BouncingBall::collide(BouncingBall& other) 
{
	const auto my_hitbox = get_hitbox();
	const auto other_hitbox = other.get_hitbox();

	const auto& v1 = _direction;
	const auto& v2 = other.get_velocity();

	const auto m1 = mass;
	const auto m2 = other.mass;

	const auto x1 = my_hitbox.position;
	const auto x2 = other_hitbox.position;

	const auto v1_calc = static_cast<float>(glm::dot(v1 - v2, x1 - x2) / glm::pow(glm::length(x1 - x2), 2));
	const auto v2_calc = static_cast<float>(glm::dot(v2 - v1, x2 - x1) / glm::pow(glm::length(x2 - x1), 2));

	const auto v1p = v1 - ((2 * m2) / (m1 + m2)) * v1_calc * (x1 - x2);
	const auto v2p = v2 - ((2 * m1) / (m1 + m2)) * v2_calc * (x2 - x1);

	set_velocity(v1p);
	other.set_velocity(v2p);
}

void game::BouncingBall::move()
{
	_origin += _direction;
}

void game::BouncingBall::init()
{
}

void game::BouncingBall::update()
{
	const auto max = game::window::get_renderer_size();

	move();

	if (_origin.x < 0) {
		_origin.x = 0;
		_direction.x = -_direction.x;
	} else if (_origin.y < 0) {
		_origin.y = 0;
		_direction.y = -_direction.y;
	} else if (_origin.x + _width > max.x) {
		_origin.x = max.x - _width;
		_direction.x = -_direction.x;
	} else if (_origin.y + _height > max.y) {
		_origin.y = max.y - _height;
		_direction.y = -_direction.y;
	}
}

void game::BouncingBall::draw() const noexcept
{
	game::window::draw_texture(_origin, _width, _height, _texture);

	const auto hitbox = get_hitbox();

	game::window::draw_circle(hitbox.position, hitbox.radius);
}

game::BouncingBall game::make_bouncing_ball(glm::vec2 origin)
{
	return game::BouncingBall{origin};
}