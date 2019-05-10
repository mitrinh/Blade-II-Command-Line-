//
// Created by PC on 7/3/2018.
//

#ifndef BLADE_HAND_H
#define BLADE_HAND_H

#include "card.h"
#include <algorithm>
#include <chrono>
#include <iostream>
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
    void printHand() const;
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
void hand::printHand() const {
    cout << "Regular Cards: " << getRegularCards() << '\t';
    cout << "Special Cards: " << getSpecialCards() << endl;
    for (auto &i : hand_) cout << i.getName() << " ";
    cout << endl;
} // end printHand

void hand::setRegularCards(unsigned int regCards){ regularCards = regCards; } // end setRegularCards

void hand::setSpecialCards(unsigned int specCards){ specialCards = specCards; } // end setSpecialCards

// sort a hand by their value
bool sortByValue(const card &lhs, const card &rhs) { return lhs.getValue() < rhs.getValue(); }

// sort a hand by their name alphabetically
bool sortByName(const card &lhs, const card &rhs) { return lhs.getName() < rhs.getName(); }

// give 10 cards to both hands from the shared deck at start of duel
void hand::createHand(stack<card> &deck) {
    unique_ptr<card> temp(&deck.top());
    for(int i = 0; i < START_HAND_QUANTITY; i++) {
        *temp = deck.top();
        if (isRegularCard(*temp)) setRegularCards(1+getRegularCards());
        else setSpecialCards(1+getSpecialCards());
        hand_.push_back(*temp);
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
