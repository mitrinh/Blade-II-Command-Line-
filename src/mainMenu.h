//
// Created by PC on 8/16/2018.
//

#ifndef BLADE_MAINMENU_H
#define BLADE_MAINMENU_H

#include "LButton.h"
#include "LWindow.h"

// used to create and destroy main menu
bool init();
bool loadMedia();
void close();

bool init() {
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Error_Logger("initialize SDL");
        success = false;
    }
    else {
        //Set texture filtering to linear
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")) SDL_Log("Warning: Linear texture filtering not enabled!");
        //Create first window
        if(!windows[0].init()) {
            SDL_Log("Window 0 could not be created!\n");
            success = false;
        }
    }
    return success;
}

bool loadMedia() {
    //Loading success flag
    bool success = true;
    //Load sprites
    if( !buttonSpriteSheetTexture.loadFromFile("images/button.png")) {
        SDL_Error_Logger("Load from file");
        success = false;
    }
    else {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i ) {
            spriteClips[i].x = 0;
            spriteClips[i].y = i * BUTTON_HEIGHT;
            spriteClips[i].w = BUTTON_WIDTH;
            spriteClips[i].h = BUTTON_HEIGHT;
        }
        // title
        buttons[0].setPosition(0,0);
        // play
        buttons[1].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, SCREEN_HEIGHT-BUTTON_HEIGHT-300);
        // quit
        buttons[2].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, SCREEN_HEIGHT-BUTTON_HEIGHT-100);
    }
    return success;
}

void close() {
    //Free loaded images
    buttonSpriteSheetTexture.free();
    //Destroy windows
    for (auto &window : windows) window.free();
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

#endif //BLADE_MAINMENU_H
