//
// Created by PC on 6/15/2018.
//

#include "duel.h"
#include "LButton.h"
#include "mainMenu.h"
#include <SDL_ttf.h>
#include <stdio.h>

/* generates each type of card */
void generateCards(card cards[]);

// prompts the user to answer the yes or no question
string askYesOrNo();

int main( int argc, char* args[] ) {
    //Start up SDL and create window
    if(!init()) SDL_Error_Logger("initialize SDL");
    else {
        for( int i = 1; i < TOTAL_WINDOWS; ++i ) windows[i].init();
        //Load media
        if(!loadMedia())SDL_Error_Logger("Load media");
        else {//Our event structure
            SDL_Event e;
            bool quit = false;
            bool startDuel = false;
            //Update all windows
            for (auto &window : windows) window.render();
            // create main menu
            while (!quit) {
                //e is an SDL_Event variable we've declared before entering the main loop
                while (SDL_PollEvent(&e) != 0 && !startDuel) {
                    //If user closes the window
                    if (e.type == SDL_QUIT) quit = true;
                    //Handle button events
                    for(int i = 0; i < TOTAL_BUTTONS; ++i) buttons[i].handleEvent(&e, i, quit, startDuel);
                    //Handle window events
                    for (auto &window : windows) window.handleEvent(e);
                    //Pull up window
                    if( e.type == SDL_KEYDOWN ) {
                        switch( e.key.keysym.sym ) {
                            case SDLK_1:
                                windows[0].focus();
                                break;
                            case SDLK_2:
                                windows[1].focus();
                                break;
                            default: break;
                        }
                    }
                    //Render buttons
                    for (auto &button : buttons) button.render();
                    //Check all windows
                    bool allWindowsClosed = true;
                    for (auto &window : windows) {
                        if(window.isShown() ) {
                            allWindowsClosed = false;
                            break;
                        }
                    }
                    //Application closed all windows
                    if(allWindowsClosed) quit = true;
                }
            }
        }
    }
    // clean up
    close();
    return 0;
} // end main

/* generates each type of card */
void generateCards(card cards[]) {
    unique_ptr<card> temp(new card);
    for (unsigned int i = 0; i < CARD_TYPES; i++) {
        // numbered cards 1-7
        if (i < 7) {
            temp->setValue(i + 1);
            temp->setName(to_string(i + 1));
        }
        // special cards
        else {
            temp->setValue(1); // can be used to check if a card is numbered or special
            switch (i) {
                case 7:
                    temp->setName("bolt");
                    temp->setCardType(1);
                    break;
                case 8:
                    temp->setName("mirror");
                    temp->setCardType(2);
                    break;
                case 9:
                    temp->setName("blast");
                    temp->setCardType(3);
                    break;
                case 10:
                    temp->setName("force");
                    temp->setCardType(4);
                    break;
                default:
                    cout << "An error has occurred. " << "cards cannot be generated";
                    break;
            }
        }
        cards[i] = *temp;
    }
} // end generateCards

// prompts the user to answer the yes or no question
string askYesOrNo() {
    string answer = "abc";
    while (answer.at(0) != 'y' && answer.at(0) != 'n') {
        cin >> answer;
        if (answer.at(0) != 'y' && answer.at(0) != 'n') cout << "Type y or n." << endl;
    }
    cout << "Choice selected: " << answer.at(0) << endl;
    return answer;
} // end askYesOrNo

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
//SDL_Texture* renderText(const string &message, const string &fontFile,
//                        SDL_Color color, int fontSize, SDL_Renderer *renderer)
//{
//    //Open the font
//    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
//    if (font == nullptr){
//        SDL_Error_Logger("TTF_OpenFont");
//        return nullptr;
//    }
//    //We need to first render to a surface as that's what TTF_RenderText
//    //returns, then load that surface into a texture
//    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
//    if (surf == nullptr){
//        TTF_CloseFont(font);
//        SDL_Error_Logger("TTF_RenderText");
//        return nullptr;
//    }
//    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
//    if (texture == nullptr) SDL_Error_Logger("CreateTexture");
//    //Clean up the surface and font
//    SDL_FreeSurface(surf);
//    TTF_CloseFont(font);
//    return texture;
//} // end renderText