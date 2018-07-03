//
// Created by PC on 7/3/2018.
//

#ifndef BLADE_HAND_H
#define BLADE_HAND_H

#include "card.h"
#include <vector>

class hand {
private:
    unsigned int regularCards;
    unsigned int specialCards;
public:
    explicit hand();
    ~hand();
    vector<card> hand_;
    unsigned int & getRegularCards();
    unsigned int & getSpecialCards();
    void setRegularCards(unsigned int);
    void setSpecialCards(unsigned int);
};

hand::hand() {
    regularCards = 0;
    specialCards = 0;
} // end constructor

hand::~hand() = default; // end destructor

unsigned int & hand::getRegularCards() { return regularCards; } // end getRegularCards

unsigned int & hand::getSpecialCards() { return specialCards; } // end getSpecialCards

void hand::setRegularCards(unsigned int regCards){ regularCards = regCards; } // end setRegularCards

void hand::setSpecialCards(unsigned int specCards){ specialCards = specCards; } // end setSpecialCards

#endif //BLADE_HAND_H
