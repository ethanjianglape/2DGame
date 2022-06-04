#pragma once

#include <memory>
#include <vector>

#include <SDL/SDL.h>
#include <glm/vec2.hpp>

#include <interfaces/IDrawable.hpp>
#include <game/Tile.hpp>
#include <entities/Player.hpp>

namespace game {
	class Level : IDrawable {
	private:
		std::vector<std::unique_ptr<Tile>> _tiles;

		std::unique_ptr<game::Player> _player;
	public:
		void generate();
		void update();
		void draw() const noexcept override;
	};
}