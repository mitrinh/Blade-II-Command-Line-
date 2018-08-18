//
// Created by PC on 8/17/2018.
//

#ifndef BLADE_LWINDOW_H
#define BLADE_LWINDOW_H

#include "errorLogger.h"
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

//Total windows
const int TOTAL_WINDOWS = 2;

//main menu window
SDL_Window* window;
//renderer
SDL_Renderer* renderer;


class LWindow{
private:
//Window dimensions
    int width;
    int height;
//Window focus
    bool mouseFocus;
    bool fullScreen;
    bool minimized{};
    bool shown;
    int windowID;
public:
//constructor
    LWindow();
//destructor
    ~LWindow();
//Starts up SDL and creates window
    bool init();
//Handles window events
    void handleEvent( SDL_Event& e );
//Focuses on window
    void focus();
//Shows windows contents
    void render();
//Deallocates internals
    void free();
//Window dimensions
    int getWidth();
    int getHeight();
//Window focus
    bool hasMouseFocus();
    bool isMinimized();
    bool isShown();
};

// constructor
LWindow::LWindow() {
    // initialize all private variables
    window = nullptr;
    renderer = nullptr;
    mouseFocus = false;
    fullScreen = false;
    shown = false;
    windowID = -1;
    width = 0;
    height = 0;
}

// destructor
LWindow::~LWindow() = default;

// initialize window, renderer, and png compatibility
bool LWindow::init() {
    //Create window
    window = SDL_CreateWindow("Blade", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( window != nullptr) {
        mouseFocus = true;
        width = SCREEN_WIDTH;
        height = SCREEN_HEIGHT;
        // create renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        // error and end if failed
        if( renderer == nullptr) {
            SDL_Error_Logger("CreateRenderer");
            SDL_DestroyWindow(window);
            window = nullptr;
        }
        else {
            //Initialize renderer color
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            // error and end if failed
            if(!(IMG_Init(imgFlags) & imgFlags)) {
                SDL_Error_Logger("initialize SDL_image");
                SDL_DestroyRenderer(renderer);
                renderer = nullptr;
                SDL_DestroyWindow(window);
                window = nullptr;
            }
            //Grab window id
            windowID = SDL_GetWindowID(window);
            //Flag window as opened
            shown = true;
        }
    }
    else SDL_Error_Logger("CreateWindow");
    return window != nullptr && renderer != nullptr;
}

// handle window events and changes window title depending on event
void LWindow::handleEvent(SDL_Event &e) {
    //If an event was detected for this window
    if( e.type == SDL_WINDOWEVENT && e.window.windowID == windowID ) {
        //Caption update flag
        // find what type of window event was done
        switch(e.window.event){
            // track if window has appeared or disappeared
            //Window appeared
            case SDL_WINDOWEVENT_SHOWN:
                shown = true;
                break;
                //Window disappeared
            case SDL_WINDOWEVENT_HIDDEN:
                shown = false;
                break;
                //Get new dimensions and repaint window
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                width = e.window.data1;
                height = e.window.data2;
                SDL_RenderPresent( renderer );
                break;
                //Repaint on expose
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent( renderer );
                break;
                //Mouse enter
            case SDL_WINDOWEVENT_ENTER:
                mouseFocus = true;
                break;
                //Mouse exit
            case SDL_WINDOWEVENT_LEAVE:
                mouseFocus = false;
                break;
                //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
                minimized = true;
                break;
                //Window maximized
            case SDL_WINDOWEVENT_MAXIMIZED:
                minimized = false;
                break;
                //Window restored
            case SDL_WINDOWEVENT_RESTORED:
                minimized = false;
                break;
                //Hide on close
            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(window);
                break;
            default: break;
        }
    }
}

// focus on a window
void LWindow::focus() {
    //Restore window if needed
    if(!shown)SDL_ShowWindow(window);
    //Move window forward
    SDL_RaiseWindow(window);
}

// render window
void LWindow::render()
{
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //Update screen
    SDL_RenderPresent(renderer);
}

// free and hide window
void LWindow::free() {
    if(window != nullptr) SDL_DestroyWindow(window);
    mouseFocus = false;
    width = 0;
    height = 0;
}

int LWindow::getWidth() { return width; }

int LWindow::getHeight() { return height; }

bool LWindow::hasMouseFocus() { return mouseFocus; }

bool LWindow::isMinimized() { return minimized; }

bool LWindow::isShown() { return shown; }


#endif //BLADE_LWINDOW_H
