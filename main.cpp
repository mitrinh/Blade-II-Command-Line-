//
// Created by PC on 6/15/2018.
//

#include "duel.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 3;

enum LButtonSprite
{
    BUTTON_SPRITE_BLADE = 0,
    BUTTON_SPRITE_PLAY = 1,
    BUTTON_SPRITE_QUIT = 2,
    BUTTON_SPRITE_TOTAL = 3
};

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void SDL_Error_Logger(const string &msg){
    SDL_Log("%s error: %s \n",msg.c_str(),SDL_GetError());
} // end SDL_Error_Logger

//Texture wrapper class
class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(const string &path );

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    //Set blending
    void setBlendMode( SDL_BlendMode blending );

    //Set alpha modulation
    void setAlpha( Uint8 alpha );

    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE );

    //Gets image dimensions
    int getWidth();
    int getHeight();

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* window = nullptr;

//The window renderer
SDL_Renderer* renderer = nullptr;

//Mouse button sprites
SDL_Rect spriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture buttonSpriteSheetTexture;

LTexture::LTexture()
{
    //Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile(const string &path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == nullptr)
    {
        SDL_Error_Logger("IMG_Load");
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == nullptr)
        {
            SDL_Log( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
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

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr)
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

//The mouse button
class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Sets top left position
    void setPosition( int, int);

    //Handles mouse event
    void handleEvent( SDL_Event*, int, bool&);

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

//spriteCount
int spriteCount = 0;

//Buttons objects
LButton buttons[ TOTAL_BUTTONS ];

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;
    mCurrentSprite = BUTTON_SPRITE_BLADE;
}

void LButton::setPosition( int x, int y )
{
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
    if( x < mPosition.x )
    {
        inside = false;
    }
//Mouse is right of the button
    else if( x > mPosition.x + BUTTON_WIDTH )
    {
        inside = false;
    }
//Mouse above the button
    else if( y < mPosition.y )
    {
        inside = false;
    }
//Mouse below the button
    else if( y > mPosition.y + BUTTON_HEIGHT )
    {
        inside = false;
    }
    return inside;
}

void LButton::handleEvent( SDL_Event* e, int index, bool &quit)
{
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
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        if(index == 2 && inside() && e->type == SDL_MOUSEBUTTONDOWN) quit = true;
    }
}

void LButton::render()
{
    //Show current button sprite
    buttonSpriteSheetTexture.render( mPosition.x, mPosition.y, &spriteClips[ mCurrentSprite ] );
}

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

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        SDL_Error_Logger("initialize SDL");
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            SDL_Error_Logger("linear texture filtering");
        }

        //Create window
        window = SDL_CreateWindow("Blade", 100, 100,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);;
        if( window == nullptr)
        {
            SDL_Error_Logger("CreateWindow");
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if( renderer == nullptr)
            {
                SDL_Error_Logger("CreateRenderer");
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    SDL_Error_Logger("initialize SDL_image");
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load sprites
    if( !buttonSpriteSheetTexture.loadFromFile("images/button.png"))
    {
        SDL_Error_Logger("loadfromfile");
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            spriteClips[ i ].x = 0;
            spriteClips[ i ].y = i * BUTTON_HEIGHT;
            spriteClips[ i ].w = BUTTON_WIDTH;
            spriteClips[ i ].h = BUTTON_HEIGHT;
        }
        // title
        buttons[ 0 ].setPosition(0,0);
        // play
        buttons[ 1 ].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, SCREEN_HEIGHT-BUTTON_HEIGHT-300);
        // quit
        buttons[ 2 ].setPosition(SCREEN_WIDTH-BUTTON_WIDTH, SCREEN_HEIGHT-BUTTON_HEIGHT-100);

    }

    return success;
}

void close()
{
    //Free loaded images
    buttonSpriteSheetTexture.free();

    //Destroy window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = nullptr;
    renderer = nullptr;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    //Start up SDL and create window
    if( !init() )
    {
        SDL_Error_Logger("initialize SDL");
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            SDL_Error_Logger("Load media");
        }
        else {//Our event structure
            SDL_Event e;
            bool quit = false;
            while (!quit) {
                //Read user input & handle it
                //e is an SDL_Event variable we've declared before entering the main loop
                while (SDL_PollEvent(&e) != 0) {
                    //If user closes the window
                    if (e.type == SDL_QUIT) quit = true;
                    //Handle button events
                    //Handle button events
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        buttons[ i ].handleEvent( &e, i, quit);
                    }
                    //Clear screen
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    //Render buttons
                    for (auto &button : buttons) {
                        button.render();
                    }
                    //Update the screen
                    SDL_RenderPresent(renderer);
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
                case 7: {
                    temp->setName("bolt");
                    temp->setCardType(1);
                    break; }
                case 8: {
                    temp->setName("mirror");
                    temp->setCardType(2);
                    break; }
                case 9: {
                    temp->setName("blast");
                    temp->setCardType(3);
                    break; }
                case 10: {
                    temp->setName("force");
                    temp->setCardType(4);
                    break; }
                default: cout << "An error has occurred. " << "cards cannot be generated";
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

