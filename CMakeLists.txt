cmake_minimum_required(VERSION 3.10)
project(Blade)

set(CMAKE_CXX_STANDARD 11)

add_executable(Blade main.cpp card.h hand.h field.h duel.h)