//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_FIELD_H
#define BLADE_FIELD_H

#include "card.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stack>
#include <vector>
#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4
#define START_HAND_QUANTITY 10

class duel {
private:
    bool turn; // turn order; true = myTurn, false = enemyTurn
    const unsigned int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    int myPile;
    int enemyPile;
    int myHandCount;
    int enemyHandCount;
    stack<card> deck;
    vector<card> myHand;
    vector<card> enemyHand;
    stack<card> myField;
    stack<card> enemyField;
    void createDeck(const card[]);
    void createHands();
    void initializeField();
public:
    explicit duel(card[]);
    ~duel();
}; // end class duel

duel::duel(card cards[]){
    myPile = 0;
    enemyPile = 0;
    myHandCount = START_HAND_QUANTITY;
    enemyHandCount = START_HAND_QUANTITY;
    while(!deck.empty()) deck.pop();
    createDeck(cards);
    createHands();
    initializeField();
} // end constructor

duel::~duel() = default; // end destructor

// creates a shared deck
void duel::createDeck(const card cards[]) {
    vector<card> temp(totalCards);
    int type = 0;
    // put in a filled deck in a temp vector
    for(unsigned int i = 0; i < totalCards; i++) {
        if ((i != 0) && (i % CARD_QUANTITY_MAX == 0)) ++type;
        temp[i] = cards[type];
    }
    // randomize that vector
    long seed = static_cast<long>(chrono::system_clock::now().time_since_epoch().count());
    shuffle(temp.begin(),temp.end(),default_random_engine(seed));
    // push the randomized vector to the deck
    for(unsigned int i = 0; i < totalCards; i++) {
        deck.push(temp.at(i));
    }
} // end duel

// give 10 cards to both hands from the shared deck at start
void duel::createHands() {
    for(int i = 0; i < START_HAND_QUANTITY; i++){
        myHand.push_back(deck.top());
        deck.pop();
        enemyHand.push_back(deck.top());
        deck.pop();
    }
}

// puts the top card from each deck onto their field
// decks drawn at same rate, so if both decks are empty, then tie
void duel::initializeField() {
    bool equalValue = true;
    while(equalValue){
        myField.push(deck.top());
        deck.pop();
        myPile += myField.top().getValue();
        enemyField.push(deck.top());
        deck.pop();
        enemyPile += enemyField.top().getValue();
        // if piles are of equal value then reset field and draw again
        if(myPile == enemyPile) {
            equalValue = true;
            myPile = enemyPile = 0;
            while(!myField.empty()) myField.pop();
            while(!enemyField.empty()) enemyField.pop();
        }
        else equalValue = false;
    }
}

#endif //BLADE_FIELD_H