#pragma once

#include <vector>
#include <chrono>

#include <entities/entity.hpp>
#include <interfaces/IControlable.hpp>
#include <game/Gamepad.hpp>

namespace game {
	enum class PlayerState {
		Idle = 0,
		Moving = 1
	};

	enum class MovementState {
		Walking = 0,
		Running = 1,
	};

	class Player : Entity {
	private:

		glm::vec2 _world_position;
		glm::vec2 _direction;
		glm::vec2 _new_direction;
		glm::vec2 _previous_direction;

		bool _changing_direction;
		bool _deccelerating;
		bool _accelerating;

		float _angle;

		float _speed;
		float _walking_speed;
		float _running_speed;
		float _acceleration;
		float _decceleration;
		float _direction_acceleration;
		float _direction_decceleration;

		PlayerState _state;
		MovementState _movement_state;

		const float get_max_speed() const noexcept;

		void button_pressed(const game::pad::ButtonState& state);
		void button_held(const game::pad::ButtonState& state);
		void button_released(const game::pad::ButtonState& state);

	public:
		Player();

		const glm::vec2 get_velocity() const noexcept;
		const glm::vec2& get_world_position() const noexcept;
		const glm::vec2 get_world_position_center() const noexcept;

		void init() override;
		void update() override;
		void draw() const noexcept override;
	};
}