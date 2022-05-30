# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2IMAGE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/SDL_Image/x64/SDL2_image.lib;")
else ()
  set(SDL2IMAGE_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/SDL_Image/x86/SDL2_image.lib;")
endif ()

string(STRIP "${SDL2IMAGE_LIBRARIES}" SDL2IMAGE_LIBRARIES)