#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <interfaces/IDrawable.hpp>
#include <texture/texture.hpp>

namespace game {
	inline constexpr float TileWidth = 32.0f;
	inline constexpr float TileHeight = 32.0f;
	inline constexpr glm::vec2 TileSize{TileWidth, TileHeight};

	enum class TileType {
		Grass = 1,
		Rock = 2,
	};

	class Tile : IDrawable {
	private:
		const TileType _type;
		const glm::vec2 _world_position;
		const bool _solid;

		std::shared_ptr<game::Texture> _texture;
		glm::vec2 _pixel_position;

	public:
		Tile(TileType type, glm::vec2 world_position);

		void draw(const glm::vec2& level_offset) const noexcept;
		void draw() const noexcept override;
	};

	std::unique_ptr<Tile> make_rock_tile(const glm::vec2& world_position);
	std::unique_ptr<Tile> make_grass_tile(const glm::vec2& world_position);
}