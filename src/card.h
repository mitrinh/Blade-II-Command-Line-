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

#ifndef BLADE_CARD_H
#define BLADE_CARD_H

#include <string>
#include <utility>
using namespace std;

class card {
private:
    bool bolted; // false = not bolted, true = bolted
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
};

card::card() {
    bolted = false;
    cardType = 0;
    value = 0;
    name = "a";
}

card::~card() = default; // end destructor

unsigned int card::getValue() const { return value; }

void card::setValue(unsigned int thisValue) { value = thisValue; }

string card::getName() const { return name; }

void card::setName(string thisName){ name = move(thisName); }

unsigned int card::getCardType() const { return cardType; }

void card::setCardType(unsigned int cardType_) { cardType = cardType_; }

bool card::getBolted() const { return bolted; }

void card::setBolted(bool bolted_) { bolted = bolted_; }

// return true if regular Card, false if not and is a special card
bool isRegularCard(const card &card_) { return (card_.getCardType() == 0); }

#endif //BLADE_CARD_H
