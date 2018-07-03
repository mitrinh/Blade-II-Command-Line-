//
// Created by PC on 7/3/2018.
//

#ifndef BLADE_FIELD_H
#define BLADE_FIELD_H

#include "card.h"
#include <stack>

class field {
private:
    unsigned int pile;
public:
    explicit field();
    ~field();
    stack<card> field_;
    unsigned int & getPile();
    void setPile(unsigned int);
};

field::field() {
    pile = 0;
}

field::~field() = default; // end destructor

unsigned int & field::getPile() { return pile; } // end getPile

void field::setPile(unsigned int amount){ pile = amount; } // end setPile

#endif //BLADE_FIELD_H
