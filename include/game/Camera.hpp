#pragma once

#include <SDL/SDL.h>

#include <glm/vec2.hpp>

namespace game::camera {
	void init();
	void begin_move(const glm::vec2& direction);
	void end_move();
	void move_offset_in_direction(const glm::vec2& direction);
	void move_offset_to_center();
	void center_on_position(const glm::vec2& position);
	void follow_position(const glm::vec2& position);
	void update();
	void draw();

	glm::vec2 get_relative_position(const glm::vec2& entity_position);
	glm::vec2 get_center_world_position();
	glm::vec2 get_offset_world_position();

	glm::vec2 get_render_offset();
}