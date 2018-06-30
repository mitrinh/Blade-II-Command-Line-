//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_FIELD_H
#define BLADE_FIELD_H

#include <algorithm>
#include "card.h"
#include <ctime>
#include <iostream>
#include <stack>
#include <vector>
#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4
using namespace std;

class duel {
private:
    bool turn = true; // turn order
    int myPile;
    int enemyPile;
    int myHandCount;
    int enemyHandCount;
    stack<card> deck;
    stack<int> myField;
    stack<int> enemyField;
    vector<int> myHand;
    vector<int> enemyHand;
    void createDeck(const card[]);
public:
    duel(card[]);
    ~duel();
};

duel::duel(card cards[]){
    myPile = 0;
    enemyPile = 0;
    myHandCount = 10;
    enemyHandCount = 10;
    while(!deck.empty()) deck.pop();
    createDeck(cards);
}

duel::~duel(){
}

// creates a shared deck
void duel::createDeck(const card cards[]) {
    int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    vector<card> temp(totalCards);
    int type = 0;
    // put in a filled deck in a temp vector
    for(int i = 0; i < totalCards; i++) {
        if ((i != 0) && (i % CARD_QUANTITY_MAX == 0)) ++type;
        temp[i] = cards[type];
    }
    // randomize that vector
    srand(time(NULL));
    random_shuffle(temp.begin(),temp.end());
    // push the randomized vector to the deck
    for(int i = 0; i < totalCards; i++) {
        deck.push(temp.at(i));
    }
    while(!deck.empty()){
        cout << deck.top().getName() << endl;
        deck.pop();
    }
}

#endif //BLADE_FIELD_H