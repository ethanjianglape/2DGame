#pragma once

#include <SDL/SDL.h>

#include <glm/vec2.hpp>

namespace game::camera {
	void init();
	void move_center_position(const glm::vec2& direction);
	void center_on_position(const glm::vec2& position);
	void follow_position(const glm::vec2& world_position);
	void update();
	void draw();

	glm::vec2 get_relative_position(const glm::vec2& entity_position);
	glm::vec2 get_world_center_position();

	glm::vec2 get_render_offset();
	const glm::vec2& get_position();
	const glm::vec2& get_size();
}