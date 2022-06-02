#pragma once

#include <SDL/SDL.h>

#include <glm/vec2.hpp>

namespace game::pad {
	enum class Joystick {
		LEFT = 0,
		RIGHT = 1,
	};

	const glm::vec2& get_joystick_direction(Joystick joystick);
	
	float get_joystick_angle(Joystick joystick);
	float get_joystick_angle_delta(Joystick joystick);

	bool is_joystick_in_deadzone(Joystick joystick);

	void init();
	void handle_event(const SDL_Event& event);
	void update();

}