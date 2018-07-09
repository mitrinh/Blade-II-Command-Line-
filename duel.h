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
#include <memory>
#include <random>
#include <stack>
#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4
#define START_HAND_QUANTITY 10

class duel {
private:
    bool equalValue;
    const unsigned int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    unsigned int player; // 1 = player 1 , 2 = player 2 , ...
    unsigned end; // 0 = lose , 1 = win, 2 = tie
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
    void playCard(hand&, hand&, field&, field&, unsigned int);
    void printHand(hand) const;
    void printDeck();
}; // end class duel

// checks if the top card is bolted
bool isBolted(const field &field__) { return (field__.field_.top().getBolted()); }

// checks if a card is force
bool isForce(const card &card_) { return card_.getCardType() == 4; }

// return true if regular Card, false if not and is a special card
bool isRegularCard(const card &card_) { return (card_.getCardType() == 0); }

// sort a hand by their value
bool sortByValue(const card &lhs, const card &rhs) { return lhs.getValue() < rhs.getValue(); }

// sort a hand by their name alphabetically
bool sortByName(const card &lhs, const card &rhs) { return lhs.getName() < rhs.getName(); }

duel::duel(card cards[]) {
    player = 1;
    end = 0;
    equalValue = true;
    while(!deck.empty()) deck.pop();
    createDeck(cards);
    createHand(hand1);
    createHand(hand2);
    sortHand(hand1);
    sortHand(hand2);
    initializeField();
    cout << "player 1: " << field1.getPile() << endl;
    cout << "player 2: " << field2.getPile() << endl;
    cout << endl;
    if (end == 2) cout << "tie" << endl;
    else if(player == 1) {
        cout << "\tplayer " << player << "'s turn.\n" << "Play a card" << endl;
        printHand(hand1);
        //playCard(hand1,hand2,field1,field2,1);
    }
    else {
        cout << "\tplayer " << player << "'s turn.\n" << "Play a card" << endl;
        printHand(hand2);
    }
//        test here
//        printDeck();
//        cout << endl;
//        printHand(hand1);
//        cout << endl;
//        printHand(hand2);
} // end constructor

duel::~duel() = default; // end destructor

// play a card from the respective player's hand
void duel::playCard(hand &hand__, hand &enemyHand, field &field__, field &enemyField, const unsigned int position) {
    unique_ptr<card> temp(&hand__.hand_.at(position));
    // put the card on the field if it has a value > 2 or if it is a '1' card and pile isn't bolted is or if it is force
    if( (temp->getValue() > 2 || (temp->getName()=="1" && !isBolted(field__)))
         || (isForce(*temp))) field__.field_.push(*temp);
    switch (temp->getCardType()) {
        case 1: { // bolt
            enemyField.field_.top().setBolted(true);
            if (isForce(enemyField.field_.top())) enemyField.setPile(enemyField.getPile() / 2);
            else enemyField.setPile(enemyField.getPile() - temp->getValue());
            break;
        }
        case 2: { // mirror
            swap(field__, enemyField);
            break;
        }
        case 3: { // blast
            // shuffle enemy's hand
            long seed = static_cast<long>(chrono::system_clock::now().time_since_epoch().count());
            shuffle(enemyHand.hand_.begin(),enemyHand.hand_.end(),default_random_engine(seed));
            cout << "Pick a card from your opponent's hand from " << 1 << " to " <<
                enemyHand.hand_.size() << " to discard.";
            unique_ptr<int>position_(new int);
            // repeats if position is out of bounds
            *position_ = 0;
            while(!((1 <= *position_) && (*position_<enemyHand.hand_.size()))){
                cin >> *position_;
                if((1 <= *position_) && (*position_<enemyHand.hand_.size()))
                    cout << "card must be between" << 1 << " and " << enemyHand.hand_.size();
            }
            // discard that card
            enemyHand.hand_.erase(enemyHand.hand_.begin() + *position_ - 1);
            // sort it back
            sortHand(enemyHand);
            break;
        }
        case 4: { // force
            field__.setPile(field__.getPile() * 2);
            break;
        }
        default: {
            if(temp->getValue()==1 && isBolted(field__)) {
                field__.field_.top().setBolted(false);
                if(isForce(field__.field_.top())) field__.setPile(field__.getPile() * 2);
                else field__.setPile(field__.getPile() + field__.field_.top().getValue());
            }
            field__.setPile(field__.getPile() + temp->getValue());
            break;
        }
    }
    // gets rid of the card from the hand
    hand__.hand_.erase(hand__.hand_.begin() + position);
} // end playCard

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
    unique_ptr<card> temp(&deck.top());
    for(int i = 0; i < START_HAND_QUANTITY; i++) {
        *temp = deck.top();
        if (isRegularCard(*temp)) hand_.setRegularCards(++hand_.getRegularCards());
        else hand_.setSpecialCards(++hand_.getSpecialCards());
        hand_.hand_.push_back(*temp);
        deck.pop();
    }
} // end createHand

// sorts a hand by 1-7 ascending cards, then special cards alphabetically
void duel::sortHand(hand &hand_) {
    unique_ptr<vector<card>> regHand(new vector<card>);
    unique_ptr<vector<card>> specialHand(new vector<card>);
    for (const auto &i : hand_.hand_) {
        if(isRegularCard(i))regHand->push_back(i);
        else specialHand->push_back(i);
    }
    sort(regHand->begin(),regHand->end(),sortByValue);
    sort(specialHand->begin(),specialHand->end(),sortByName);
    regHand->insert(regHand->end(),specialHand->begin(),specialHand->end());
    hand_.hand_.swap(*regHand);
} // end sortHand

// output a hand
void duel::printHand(const hand hand__) const {
    for (auto &i : hand__.hand_) cout << i.getName() << " ";
    cout << endl;
} // end printHand

// output a deck
void duel::printDeck() {
    while(!deck.empty()){
        cout << deck.top().getName() << endl;
        deck.pop();
    }
} // end printHand

// puts the top card from each deck onto their field
// decks drawn at same rate, so if both decks are empty, then tie
void duel::initializeField() {

    while(equalValue){
        // checks if deck empty, then tie
        if(deck.empty()) {
            end = 2;
            break;
        }
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
        else {
            equalValue = false;
            // player 1 goes first if pile is less than player 2, else player 2's turn
            player = (field1.getPile() < field2.getPile()) ? 1 : 2;
        }
    }
} // end initializeField

#endif //BLADE_DUEL_H