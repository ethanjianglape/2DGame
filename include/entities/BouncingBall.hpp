#pragma once

#include <memory>
#include <array>

#include <entities/entity.hpp>
#include <texture/texture.hpp>

#include <SDL/SDL.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace game {
	class Hitbox {
	public:
		glm::vec2 position;
		float radius;
	};

	class BouncingBall : Entity {
	public:

		glm::vec2 _direction;

		BouncingBall();
		BouncingBall(glm::vec2 origin);

		bool collides_with(const BouncingBall& other);
		void collision_check(BouncingBall& other);

		float mass = 1.0f;

		void set_velocity(glm::vec2 velocity);
		const glm::vec2& get_velocity() const;
		const glm::vec2& get_position() const;
		const glm::vec2 get_size() const;

		const Hitbox get_hitbox() const;

		void move();

		void collide(BouncingBall& other);

		void init() override;
		void update() override;
		void draw() const noexcept override;
	};

	BouncingBall make_bouncing_ball(glm::vec2 origin);
}