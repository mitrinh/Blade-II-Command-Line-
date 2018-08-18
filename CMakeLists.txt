cmake_minimum_required(VERSION 3.10)
project(Blade)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
add_executable(Blade main.cpp duel.h card.h hand.h field.h LTexture.h errorLogger.h LButton.h mainMenu.h LWindow.h)
target_link_libraries(Blade ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY}-lSDL2_image -lSDL2_ttf)
#-lmingw32 -lSDL2main -lSDL2