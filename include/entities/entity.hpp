#pragma once
#ifndef _ENTITY_HPP
#define _ENTITY_HPP

#include <memory>
#include <string>
#include <functional>

#include <texture/texture.hpp>
#include <interfaces/IDrawable.hpp>
#include <interfaces/IUpdatable.hpp>
#include <interfaces/IInitializable.hpp>

#include <SDL/SDL.h>

#include <glm/vec2.hpp>

namespace game {
	class Entity : IDrawable, IUpdatable, IInitializable {
	protected:
		std::shared_ptr<Texture> _texture;

		glm::vec2 _origin;

		float _width;
		float _height;

	public:
		virtual void init() = 0;
		virtual void update() = 0;
		virtual void draw() const noexcept = 0;
	};
}

#endif