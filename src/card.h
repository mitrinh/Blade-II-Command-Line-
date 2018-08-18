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
    name = "empty";
}

card::~card() = default; // end destructor

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
