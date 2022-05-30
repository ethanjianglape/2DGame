#pragma once

#include <game/Tile.hpp>
#include <window/window.hpp>
#include <texture/texture.hpp>

std::unique_ptr<game::Tile> game::make_rock_tile(const glm::vec2& world_position)
{
	return std::make_unique<game::Tile>(game::TileType::Rock, world_position);
}

std::unique_ptr<game::Tile> game::make_grass_tile(const glm::vec2& world_position)
{
	return std::make_unique<game::Tile>(game::TileType::Grass, world_position);
}

constexpr std::string_view get_tile_texture_uri(const game::TileType type)
{
	switch (type) {
	case game::TileType::Grass:
		return "images/grass.png";
	case game::TileType::Rock:
		return "images/rock.png";
	default:
		return "";
	}
}

constexpr bool get_tile_solid(const game::TileType type) {
	switch (type) {
	case game::TileType::Grass:
		return false;
	case game::TileType::Rock:
		return true;
	default:
		return false;
	}
}

game::Tile::Tile(TileType type, glm::vec2 world_position) 
	: _type{type}, 
	  _world_position{world_position},
	  _solid{get_tile_solid(type)}
{
	_texture = game::make_texture(get_tile_texture_uri(type));
	_pixel_position = glm::vec2{world_position.x * TileWidth, world_position.y * TileHeight};
}

void game::Tile::draw(const glm::vec2& level_offset) const noexcept
{
	game::window::draw_texture(_pixel_position + level_offset, TileSize, _texture);
}

void game::Tile::draw() const noexcept
{
	game::window::draw_texture(_pixel_position, TileSize, _texture);
}