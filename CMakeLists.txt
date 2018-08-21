cmake_minimum_required(VERSION 3.10)
project(Blade)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/main.cpp src/card.h src/hand.h src/field.h src/duel.h)

add_executable(Blade ${SOURCE_FILES})