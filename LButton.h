//
// Created by PC on 8/16/2018.
//

#ifndef BLADE_LBUTTON_H
#define BLADE_LBUTTON_H

#include "LTexture.h"
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 3;

enum LButtonSprite {
    BUTTON_SPRITE_BLADE = 0,
    BUTTON_SPRITE_PLAY = 1,
    BUTTON_SPRITE_QUIT = 2,
    BUTTON_SPRITE_TOTAL = 3
};

//Mouse button sprites
SDL_Rect spriteClips[BUTTON_SPRITE_TOTAL];

// buttonSpriteSheetTexture
LTexture buttonSpriteSheetTexture;

//The mouse button
class LButton {
public:
    //Initializes internal variables
    LButton();
    //Sets top left position
    void setPosition( int, int);
    //Handles mouse event
    void handleEvent( SDL_Event*, int, bool&, bool&);
    //Shows button sprite
    void render();
private:
    //checks if mouse is inside button
    bool inside();
    //Top left position
    SDL_Point mPosition{};
    //Currently used global sprite
    LButtonSprite mCurrentSprite;
};

// array of buttons
LButton buttons[TOTAL_BUTTONS];

LButton::LButton() {
    mPosition.x = 0;
    mPosition.y = 0;
    mCurrentSprite = BUTTON_SPRITE_BLADE;
}

void LButton::setPosition( int x, int y ) {
    mPosition.x = x;
    mPosition.y = y;
}

bool LButton::inside() {
//Get mouse position
    int x, y;
    SDL_GetMouseState( &x, &y );

//Check if mouse is in button
    bool inside = true;
//Mouse is left of the button
    if( x < mPosition.x )inside = false;
//Mouse is right of the button
    else if( x > mPosition.x + BUTTON_WIDTH ) inside = false;
//Mouse above the button
    else if( y < mPosition.y ) inside = false;
//Mouse below the button
    else if( y > mPosition.y + BUTTON_HEIGHT ) inside = false;
    return inside;
}

// handles events regarding buttons
void LButton::handleEvent( SDL_Event* e, int index, bool &quit, bool &startDuel) {
    switch(index) {
        case 0:
            mCurrentSprite = BUTTON_SPRITE_BLADE;
            break;
        case 1:
            mCurrentSprite = BUTTON_SPRITE_PLAY;
            break;
        case 2:
            mCurrentSprite = BUTTON_SPRITE_QUIT;
            break;
        default:break;
    }
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN) {
        // Play button
        if(index == 1 && inside() && e->type == SDL_MOUSEBUTTONDOWN){
            startDuel = true;
        }
        // quit button
        if(index == 2 && inside() && e->type == SDL_MOUSEBUTTONDOWN) quit = true;
    }
}

//Show current button sprite
void LButton::render() { buttonSpriteSheetTexture.render( mPosition.x, mPosition.y, &spriteClips[ mCurrentSprite ] );
}

#endif //BLADE_LBUTTON_H
