# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.8)

project(Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include sub-projects.
add_subdirectory("src")

add_executable(Game "src/main.cpp" "src/window/window.cpp" "src/surface/surface.cpp" "include/game/game.hpp" "src/game/game.cpp" "include/texture/texture.hpp" "src/texture/texture.cpp" "src/entities/BouncingBall.cpp" "include/entities/entity.hpp"  "include/interfaces/IDrawable.hpp" "include/interfaces/IUpdatable.hpp" "include/entities/BouncingBall.hpp" "include/interfaces/IControlable.hpp" "src/entities/Player.cpp" "include/entities/Player.hpp" "GameState.hpp" "include/game/Level.hpp" "include/game/Tile.hpp" "src/game/Tile.cpp" "src/game/Level.cpp" "include/game/Camera.hpp" "src/game/Camera.cpp"  "include/game/Gamepad.hpp" "src/game/Gamepad.cpp")

file(COPY "${CMAKE_CURRENT_LIST_DIR}/images" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# SDL2
set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SDL2IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(GLM_DIR ${CMAKE_CURRENT_LIST_DIR})
find_package(SDL2 REQUIRED)
find_package(SDL2IMAGE REQUIRED)
find_package(GLM REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(Game ${SDL2_LIBRARIES})
target_link_libraries(Game ${SDL2IMAGE_LIBRARIES})