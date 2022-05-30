#pragma once
#ifndef _IDRAWABLE_HPP
#define _IDRAWABLE_HPP

namespace game {
	class IDrawable {
	public:
		virtual void draw() const noexcept = 0;
	};
}

#endif