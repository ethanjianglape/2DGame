#pragma once

#include <vector>
#include <chrono>

#include <entities/entity.hpp>
#include <interfaces/IControlable.hpp>

namespace game {
	class Player : Entity, IControlable {
	private:
		void handle_key_up(const SDL_Event& event);
		void handle_key_down(const SDL_Event& event);

		glm::vec2 _world_position;
		glm::vec2 _direction;
		glm::vec2 _new_direction;
		glm::vec2 _previous_direction;

		std::vector<glm::vec2> _previous_directions;

		std::chrono::high_resolution_clock::time_point _last_directions_clear;

		bool _left_joystick_x_set;
		bool _left_joystick_y_set;
		bool _changing_direction;
		bool _deccelerating;
		bool _accelerating;

		float _left_joystick_x;
		float _left_joystick_y;
		float _right_joystick_x;
		float _right_joystick_y;
		float _angle;

		float _speed;
		float _walking_speed;
		float _running_speed;
		float _acceleration;
		float _decceleration;
		float _direction_acceleration;
		float _direction_decceleration;

		bool _running;
		bool _moving;

		glm::vec2 _left_joystick;
		glm::vec2 _right_joystick;

		const float get_max_speed() const noexcept;

		void handle_left_joystick(const SDL_Event& event);
		void handle_right_joystick(const SDL_Event& event);
		void handle_button_down(const SDL_Event& event);
		void handle_button_up(const SDL_Event& event);

	public:
		Player();

		const glm::vec2 get_velocity() const noexcept;
		const glm::vec2& get_world_position() const noexcept;
		const glm::vec2 get_world_position_center() const noexcept;

		void init() override;
		void handle_keypress(const SDL_Event& event) override;
		void handle_joystick(const SDL_Event& event);
		void update() override;
		void draw() const noexcept override;
	};
}