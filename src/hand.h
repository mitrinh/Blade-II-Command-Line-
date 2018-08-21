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

#ifndef BLADE_HAND_H
#define BLADE_HAND_H

#include "card.h"
#include <algorithm>
#include <chrono>
#include <memory>
#include <random>
#include <stack>
#include <vector>

#define START_HAND_QUANTITY 10

class hand {
private:
    unsigned int regularCards;
    unsigned int specialCards;
public:
    vector<card> hand_;
    explicit hand();
    ~hand();
    unsigned int getRegularCards() const;
    unsigned int getSpecialCards() const;
    void setRegularCards(unsigned int);
    void setSpecialCards(unsigned int);
    void createHand(stack<card>&);
    string printHand();
    string printHidden();
    void shuffleHand();
    void sortHand();
};

hand::hand() {
    regularCards = 0;
    specialCards = 0;
} // end constructor

hand::~hand() = default; // end destructor

unsigned int hand::getRegularCards() const { return regularCards; } // end getRegularCards

unsigned int hand::getSpecialCards() const { return specialCards; } // end getSpecialCards

// output hand
string hand::printHand() {
    string output;
    for (auto &card : hand_) output += "[" + card.getName() + "] ";
    return output;
} // end printHand

// output hidden version of hand
string hand::printHidden() {
    string output;
    for (auto &card : hand_) output += "[?] ";
    return output;
} // end printHidden

void hand::setRegularCards(unsigned int regCards){ regularCards = regCards; } // end setRegularCards

void hand::setSpecialCards(unsigned int specCards){ specialCards = specCards; } // end setSpecialCards

// sort a hand by their value
bool sortByValue(const card &lhs, const card &rhs) { return lhs.getValue() < rhs.getValue(); }

// sort a hand by their name alphabetically
bool sortByName(const card &lhs, const card &rhs) { return lhs.getName() < rhs.getName(); }

// give 10 cards to both hands from the shared deck at start of duel
void hand::createHand(stack<card> &deck) {
    card temp;
    for(int i = 0; i < START_HAND_QUANTITY; i++) {
        temp = deck.top();
        if (isRegularCard(temp)) setRegularCards(1+getRegularCards());
        else setSpecialCards(1+getSpecialCards());
        hand_.push_back(temp);
        deck.pop();
    }
} // end createHand

// shuffle hand
void hand::shuffleHand() {
    long seed = static_cast<long>(chrono::system_clock::now().time_since_epoch().count());
    shuffle(hand_.begin(),hand_.end(),default_random_engine(seed));
}

// sorts a hand by 1-7 ascending cards, then special cards alphabetically
void hand::sortHand() {
    unique_ptr <vector<card>> regHand(new vector<card>);
    unique_ptr <vector<card>> specialHand(new vector<card>);
    for (auto &i : hand_) {
        if(isRegularCard(i))regHand->push_back(i);
        else specialHand->push_back(i);
    }
    sort(regHand->begin(),regHand->end(),sortByValue);
    sort(specialHand->begin(),specialHand->end(),sortByName);
    regHand->insert(regHand->end(),specialHand->begin(),specialHand->end());
    hand_.swap(*regHand);
} // end sortHand

#endif //BLADE_HAND_H
