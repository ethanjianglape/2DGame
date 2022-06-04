#pragma once

#include <functional>

#include <SDL/SDL.h>

#include <glm/vec2.hpp>

namespace game::pad {
	enum class Joystick {
		Left = 0,
		Right = 1,
	};

	enum class Button {
		Invalid = -1,
		Cross = 0,
		Circle = 1,
		Square = 2,
		Triangle = 3,
		L3 = 7,
		R3 = 8,
		L1 = 9,
		R1 = 10,
		Up = 11,
		Down = 12,
		Left = 13,
		Right = 14
	};

	enum class ButtonAction {
		None = 0,
		Pressed = 1,
		Held = 2,
		Released = 3,
	};

	struct ButtonState {
		const Button button;
		ButtonAction action;
		std::chrono::steady_clock::time_point press_start_time;
		std::chrono::milliseconds held_duration;

		ButtonState() : ButtonState(Button::Invalid, ButtonAction::None) {}

		ButtonState(Button _button, ButtonAction _action) : 
			button{_button}, 
			action{_action},
			held_duration{0}
		{};
	};

	struct ButtonActionHandler {
		const ButtonAction action;
		const std::function<void(const ButtonState&)> handler;
	};

	const glm::vec2& get_joystick_direction(Joystick joystick);
	
	float get_joystick_angle(Joystick joystick);
	float get_joystick_angle_delta(Joystick joystick);

	bool is_joystick_in_deadzone(Joystick joystick);

	void init();
	void handle_event(const SDL_Event& event);
	void update();

	void add_button_action_handler(ButtonActionHandler handler);
}