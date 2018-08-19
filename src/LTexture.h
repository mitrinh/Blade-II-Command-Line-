//
// Created by PC on 8/16/2018.
//

#ifndef BLADE_LTEXTURE_H
#define BLADE_LTEXTURE_H

#include "errorLogger.h"
#include "LWindow.h"
#include <SDL_image.h>

//surface
SDL_Surface* loadedSurface = nullptr;

//Texture wrapper class
class LTexture : public LWindow {
private:
    //The actual hardware texture
    SDL_Texture* mTexture;
    //Image dimensions
    int mWidth;
    int mHeight;
public:
    //Initializes variables
    LTexture();
    //Deallocates memory
    ~LTexture();
    //Loads image at specified path
    bool loadFromFile(const string &);
    //Deallocates texture
    void texFree();
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    //Set blending
    void setBlendMode( SDL_BlendMode blending );
    //Set alpha modulation
    void setAlpha( Uint8 alpha );
    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr,
            SDL_RendererFlip flip = SDL_FLIP_NONE );
    //Gets image dimensions
    int getTexWidth();
    int getTexHeight();
};

LTexture::LTexture() {
    //Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

//Deallocate
LTexture::~LTexture() { free(); }

bool LTexture::loadFromFile(const string &path) {
    //Get rid of preexisting texture
    free();
    //The final texture
    SDL_Texture* newTexture = nullptr;
    //Load image at specified path
    loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == nullptr) SDL_Error_Logger("IMG_Load");
    else {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(windows->renderer, loadedSurface);
        if(newTexture == nullptr) SDL_Error_Logger("createTextureFromSurface");
        else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    //Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}

void LTexture::texFree() {
    //Free texture if it exists
    if( mTexture != nullptr) {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

//Modulate texture rgb
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ) { SDL_SetTextureColorMod( mTexture, red, green, blue ); }

//Set blending function
void LTexture::setBlendMode( SDL_BlendMode blending ) { SDL_SetTextureBlendMode( mTexture, blending );
}

//Modulate texture alpha
void LTexture::setAlpha( Uint8 alpha ) { SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    //Set clip rendering dimensions
    if( clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    //Render to screen
    SDL_RenderCopyEx(windows->renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getTexWidth() { return mWidth; }

int LTexture::getTexHeight() { return mHeight; }

#endif //BLADE_LTEXTURE_H
