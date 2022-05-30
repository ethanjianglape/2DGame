// SpaceGame.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <chrono>

#include <SDL/SDL.h>

#include <game/game.hpp>

int main(int argc, char** argv) {
	game::init();
	game::loop();

	return 0;
}
