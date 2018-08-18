//
// Created by PC on 8/16/2018.
//

#ifndef BLADE_ERRORLOGGER_H
#define BLADE_ERRORLOGGER_H

#include <SDL.h>
#include <string>
using namespace std;
/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void SDL_Error_Logger(const string &msg){
    SDL_Log("%s error: %s \n",msg.c_str(),SDL_GetError());
} // end SDL_Error_Logger

#endif //BLADE_ERRORLOGGER_H
