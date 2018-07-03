//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_DUEL_H
#define BLADE_DUEL_H

#include "field.h"
#include "hand.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stack>
#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4
#define START_HAND_QUANTITY 10

class duel {
private:
    unsigned int turnOrder{}; // turn order; 1 = player 1 , 2 = player 2 , ...
    bool isRegularCard(const card &);
    const unsigned int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    stack<card> deck;
    field field1;
    field field2;
    hand hand1;
    hand hand2;
    void createDeck(const card[]);
    void createHand(hand&);
    void initializeField();
    void sortHand(hand&);
public:
    explicit duel(card[]);
    ~duel();
    void printHand(hand);
    void printDeck();
}; // end class duel

bool sortByValue(const card &lhs, const card &rhs) { return lhs.getValue() < rhs.getValue(); }
bool sortByName(const card &lhs, const card &rhs) { return lhs.getName() < rhs.getName(); }

duel::duel(card cards[]) {
    while(!deck.empty()) deck.pop();
    createDeck(cards);
    createHand(hand1);
    createHand(hand2);
    sortHand(hand1);
    sortHand(hand2);
    initializeField();
    // test here
    printDeck();
    cout << endl;
    printHand(hand1);
    cout << endl;
    printHand(hand2);
} // end constructor

duel::~duel() = default; // end destructor

// return true if regular Card, false if not and is a special card
bool duel::isRegularCard(const card &card_) { return (card_.getName() == "1" || card_.getValue() > 1); }

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
void duel::createHand(hand &hand_) {
    card *temp;
    for(int i = 0; i < START_HAND_QUANTITY; i++) {
        temp = new card;
        *temp = deck.top();
        if (isRegularCard(*temp)) hand_.setRegularCards(++hand_.getRegularCards());
        else hand_.setSpecialCards(++hand_.getSpecialCards());
        hand_.hand_.push_back(*temp);
        deck.pop();
        delete temp;
    }
} // end createHand

// sorts a hand by 1-7 ascending cards, then special cards alphabetically
void duel::sortHand(hand &hand_) {
    auto *regHand = new vector<card>;
    auto *specialHand = new vector<card>;
    for (const auto &i : hand_.hand_) {
        if(isRegularCard(i))regHand->push_back(i);
        else specialHand->push_back(i);
    }
    sort(regHand->begin(),regHand->end(),sortByValue);
    sort(specialHand->begin(),specialHand->end(),sortByName);
    regHand->insert(regHand->end(),specialHand->begin(),specialHand->end());
    hand_.hand_.swap(*regHand);
    delete regHand;
    delete specialHand;
} // end sortHand

// output a hand
void duel::printHand(const hand hand_){
    for (auto &i : hand_.hand_) cout << i.getName() << endl;
} // end printHand

// output a deck
void duel::printDeck(){
    while(!deck.empty()){
        cout << deck.top().getName() << endl;
        deck.pop();
    }
} // end printHand

// puts the top card from each deck onto their field
// decks drawn at same rate, so if both decks are empty, then tie
void duel::initializeField() {
    bool equalValue = true;
    while(equalValue){
        field1.field_.push(deck.top());
        deck.pop();
        field1.setPile(field1.getPile()+field1.field_.top().getValue());
        field2.field_.push(deck.top());
        deck.pop();
        field2.setPile(field2.getPile()+field2.field_.top().getValue());
        // if piles are of equal value then reset field and draw again
        if(field1.getPile() == field2.getPile()) {
            equalValue = true;
            field1.setPile(0);
            field2.setPile(0);
            while(!field1.field_.empty()) field1.field_.pop();
            while(!field2.field_.empty()) field2.field_.pop();
        }
        else equalValue = false;
    }
} // end initializeField

#endif //BLADE_DUEL_H