#pragma once
#ifndef _GAME_HPP
#define _GAME_HPP

#include <memory>

#include <window/window.hpp>
#include <surface/surface.hpp>
#include <texture/texture.hpp>
#include <entities/BouncingBall.hpp>

namespace game {
	void init();
	void loop();
}

#endif