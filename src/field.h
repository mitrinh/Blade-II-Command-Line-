//
//    Name:         Trinh, Michael
//    Project:      Blade
//    Date Started: 06/15/18
//    Date Finished:08/21/18
//
//    Description:      A card game based on the minigame in the "Trails" video game franchise.
//                  -The game is set up with a deck of 44 cards and is shared between 2 players holding 10 cards taken
//                      10 cards from the deck.
//                  -The cards have their own value or score whenever they are played.
//                  -At the start of the game, both players play a card from the top of the deck and whoever's score is
//                      lower goes first.
//                  -The win condition is to "checkmate" the other  player so that their score is lower than yours after
//                      they play a card.
//                  -Whenever both scores are equal,clear all played cards and current scores then start the game again.
//                  -There are 2 types of cards: regular cards of values 1-7, and special cards which have a base value
//                      of 1 when played from the deck or an effect depending on the card when played from the hand.
//
//                  -There are 4 different special cards: blast, bolt, force, and mirror.
//                      -blast lets you choose a random card from your opponent's hand and discard, then lets you play
//                          again.
//                      -bolt negates the opponent's last played card's value if it is regular or force. Cards that have
//                          been bolted can be restored by a regular "1" card.
//                      -force doubles the score of your played cards.
//                      -mirror swaps the opponent's played cards and score with yours.

#ifndef BLADE_FIELD_H
#define BLADE_FIELD_H

#include "card.h"
#include <vector>

class field {
private:
    unsigned int pile;
public:
    explicit field();
    ~field();
    vector<card> field_;
    unsigned int & getPile();
    void setPile(unsigned int);
    void clearField();
    void resetPile();
    string printPile();
};

field::field() {
    pile = 0;
}

field::~field() = default; // end destructor

unsigned int & field::getPile() { return pile; } // end getPile

void field::setPile(unsigned int amount){ pile = amount; } // end setPile

// gets rid of each card on field, if a card is bolted; unbolt it.
void field::clearField() {
    if(field_.back().getBolted()) field_.back().setBolted(false);
    while(!field_.empty()) field_.erase(field_.begin() + field_.size() - 1);
} // end clearField

void field::resetPile() { pile = 0; } // end resetPile

string field::printPile() {
    string output;
    for(auto &card : field_) {
        output += "[" + card.getName();
        if (card.getBolted()) output += "(bolted)";
        output += "] ";
    }

    return output;
} // end printField

#endif //BLADE_FIELD_H
