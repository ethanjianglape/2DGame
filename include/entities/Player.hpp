#pragma once

#include <entities/entity.hpp>
#include <interfaces/IControlable.hpp>

namespace game {
	class Player : Entity, IControlable {
	private:
		void handle_key_up(const SDL_Event& event);
		void handle_key_down(const SDL_Event& event);

		glm::vec2 _world_position;
		glm::vec2 _direction;

		float _left_joystick_x;
		float _left_joystick_y;
		float _right_joystick_x;
		float _right_joystick_y;

		float _speed;
		float _walking_speed;
		float _running_speed;
		float _acceleration;
		float _decceleration;

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