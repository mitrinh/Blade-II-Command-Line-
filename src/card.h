//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_CARD_H
#define BLADE_CARD_H

#include "LButton.h"
#include <string>
#include <utility>

const int CARD_WIDTH = 100;
const int CARD_HEIGHT = 150;
const int TOTAL_CARDS = 20;

enum cardSprite {
    CARD_SPRITE_1 = 0,
    CARD_SPRITE_2 = 1,
    CARD_SPRITE_3 = 2,
    CARD_SPRITE_4 = 3,
    CARD_SPRITE_5 = 4,
    CARD_SPRITE_6 = 5,
    CARD_SPRITE_7 = 6,
    CARD_SPRITE_BLAST = 7,
    CARD_SPRITE_BOLT = 8,
    CARD_SPRITE_FORCE = 9,
    CARD_SPRITE_MIRROR = 10,
    CARD_SPRITE_TOTAL = 11
};

class card {
private:
    bool bolted; // false = not bolted, true = bolted
    cardSprite cardCurrentSprite;
    SDL_Point cardPosition;
    unsigned int cardType; // 0 = regCard, 1 = bolt, 2 = mirror, 3 = blast, 4 = force
    unsigned int value;
    string name;
public:
    card();
    ~card();
    unsigned int getValue() const;
    void setValue(unsigned int);
    string getName() const;
    void setName(string);
    unsigned int getCardType() const;
    void setCardType(unsigned int);
    bool getBolted() const;
    void setBolted(bool);
    void setPosition(int,int);
    bool inside();
    void handleEvent(SDL_Event*);
    void render();
};

SDL_Rect cardSpriteClips[CARD_SPRITE_TOTAL];

LTexture cardSpriteSheetTexture;

// array of card types
LButton cards[TOTAL_CARDS];

card::card() {
    bolted = false;
    cardType = 0;
    value = 0;
    name = "empty";
    cardPosition.x = 0;
    cardPosition.y = 0;
    cardCurrentSprite = CARD_SPRITE_1;
}

card::~card() = default; // end destructor

// set position of the card
void card::setPosition(int x,int y) {
    cardPosition.x = x;
    cardPosition.y = y;
}

// check if mouse is inside card texture
bool card::inside() {
//Get mouse position
    int x, y;
    SDL_GetMouseState(&x, &y);
//set flag if mouse is in card
    bool inside = true;
//Mouse is left of the card
    if(x < cardPosition.x )inside = false;
//Mouse is right of the card
    else if(x > cardPosition.x + CARD_WIDTH) inside = false;
//Mouse above the card
    else if(y < cardPosition.y) inside = false;
//Mouse below the card
    else if(y > cardPosition.y + CARD_HEIGHT) inside = false;
    return inside;
}

// handle event of changing to right cardSprite and when they are clicked
void card::handleEvent(SDL_Event * e) {
    switch(getValue()) {
        case 2:
            cardCurrentSprite = CARD_SPRITE_2;
            break;
        case 3:
            cardCurrentSprite = CARD_SPRITE_3;
            break;
        case 4:
            cardCurrentSprite = CARD_SPRITE_4;
            break;
        case 5:
            cardCurrentSprite = CARD_SPRITE_5;
            break;
        case 6:
            cardCurrentSprite = CARD_SPRITE_6;
            break;
        case 7:
            cardCurrentSprite = CARD_SPRITE_7;
            break;
        // assume value is 1
        default:
            // 0 = '1', 1 = bolt, 2 = mirror, 3 = blast, 4 = force
            switch(getCardType()){
                case 0:
                    cardCurrentSprite = CARD_SPRITE_1;
                    break;
                case 1:
                    cardCurrentSprite = CARD_SPRITE_BOLT;
                    break;
                case 2:
                    cardCurrentSprite = CARD_SPRITE_MIRROR;
                    break;
                case 3:
                    cardCurrentSprite = CARD_SPRITE_BLAST;
                    break;
                case 4:
                    cardCurrentSprite = CARD_SPRITE_FORCE;
                    break;
                default: break;
            }
            break;
    }
    if(inside() && e->type == SDL_MOUSEBUTTONDOWN) {
        cardPosition.x = 320;
        cardPosition.y = 240;
    }
}

// shows card sprite
void card::render() { cardSpriteSheetTexture.render(cardPosition.x,cardPosition.y,&cardSpriteClips[cardCurrentSprite]);}

unsigned int card::getValue() const { return value; }

void card::setValue(unsigned int thisValue) { value = thisValue; }

string card::getName() const { return name; }

void card::setName(string thisName){ name = std::move(thisName); }

unsigned int card::getCardType() const { return cardType; }

void card::setCardType(unsigned int cardType_) { cardType = cardType_; }

bool card::getBolted() const { return bolted; }

void card::setBolted(bool bolted_) { bolted = bolted_; }

// return true if regular Card, false if not and is a special card
bool isRegularCard(const card &card_) { return (card_.getCardType() == 0); }

#endif //BLADE_CARD_H
