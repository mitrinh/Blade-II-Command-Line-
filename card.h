//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_CARD_H
#define BLADE_CARD_H

#include <string>
#include <utility>
using namespace std;

class card {
private:
    unsigned int value;
    string name;
public:
    card();
    ~card();
    unsigned int getValue() const;
    void setValue(unsigned int thisValue);
    string getName() const;
    void setName(string thisName);
};

card::card() {
    value = 0;
    name = "empty";
}

card::~card() = default; // end destructor

unsigned int card::getValue() const { return value; }

void card::setValue(unsigned int thisValue) { value = thisValue; }

string card::getName() const { return name; }

void card::setName(string thisName){ name = std::move(thisName); }

#endif //BLADE_CARD_H
