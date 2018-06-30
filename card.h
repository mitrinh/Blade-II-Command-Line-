//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_CARD_H
#define BLADE_CARD_H

#include <string>
using namespace std;

class card {
private:
    int value;
    string name;
public:
    card();
    int getValue();
    void setValue(int thisValue);
    string getName();
    void setName(string thisName);
};

card::card() {
    value = -1;
    name = "empty";
}

int card::getValue() {
    return value;
}

void card::setValue(int thisValue) {
    value = thisValue;
}

string card::getName() {
    return name;
}

void card::setName(string thisName){
    name = thisName;
}


#endif //BLADE_CARD_H
