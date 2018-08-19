//
// Created by Michael Trinh
//

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
