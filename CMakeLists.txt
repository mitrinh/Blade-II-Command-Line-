cmake_minimum_required(VERSION 3.10)
project(Blade)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
set(SOURCE_FILES
        src/main.cpp
        src/duel.h
        src/card.h
        src/hand.h
        src/field.h
        src/LTexture.h
        src/errorLogger.h
        src/LButton.h
        src/mainMenu.h
        src/LWindow.h)
add_executable(Blade ${SOURCE_FILES})
target_link_libraries(Blade ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY}-lSDL2_image -lSDL2_ttf)
#-lmingw32 -lSDL2main -lSDL2