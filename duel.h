//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_DUEL_H
#define BLADE_DUEL_H

#include "field.h"
#include "hand.h"
#include <cstdlib>
#include <cctype>

#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4


class duel {
private:
    bool duelEnd; // shows when duel is over
    bool equalValue();
    const unsigned int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    unsigned int player; // 1 = player 1 , 2 = player 2 , ...
    stack<card> deck;
    field field1;
    field field2;
    hand hand1;
    hand hand2;
    void checkEnd();
    void createDeck(const card[]);
    void discardCard(unsigned int, hand&);
    void initializeField();
    void initializeField_();
    void initializeHands();
    void resetField();
    unsigned int pickCard(hand&); // returns the chosen position of the card from a player's hand
public:
    explicit duel(card[]);
    ~duel();
    void playCard(hand&, hand&, field&, field&, unsigned int);
    void printDeck();
    void printScore();
    void printTopFields();
}; // end class duel

// returns whether both piles are of equal value or not
bool duel::equalValue() { return field1.getPile() == field2.getPile();}

// checks if the top card is bolted
bool isBolted(const field &field__) { return (field__.field_.top().getBolted()); }

// checks if a card is force
bool isForce(const card &card_) { return card_.getCardType() == 4; }

// check if a string is a number
bool is_number(const std::string& s) {
    return !s.empty() && find_if(s.begin(),s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

// constructor
duel::duel(card cards[]) {
    player = 1;
    duelEnd = false;
    while(!deck.empty()) deck.pop();
    createDeck(cards);
    initializeHands();
    // sets up field by drawing until tie is gone
    initializeField();
    // player 1 goes first if pile is less than player 2, else player 2's turn
    player = (field1.getPile() < field2.getPile()) ? 1 : 2;
    while(!duelEnd) {
        cout << "player " << player << "'s turn." << endl;
        if(player == 1) {
            playCard(hand1,hand2,field1,field2,pickCard(hand1));
            // check if lost
            if (field2.getPile() > field1.getPile()) duelEnd = true;
            player = 2;
        }
        else {
            playCard(hand2,hand1,field2,field1,pickCard(hand2));
            // check if lost
            if (field1.getPile() > field2.getPile()) duelEnd = true;
            player = 1;
        }
        if (equalValue()) {
            resetField();
            initializeField();
            player = (field1.getPile() < field2.getPile()) ? 1 : 2;
        }
    }
    checkEnd();
} // end constructor

duel::~duel() = default; // end destructor

// play a card from the respective player's hand
void duel::playCard(hand &hand__, hand &enemyHand, field &field__, field &enemyField, unsigned int position) {
    // check if you only have special cards in hand
    if(hand__.getSpecialCards() == 0) duelEnd = true;
    else {
        // create pointer to card that is played
        unique_ptr<card> temp(&hand__.hand_.at(position));
        cout << temp->getName() << endl;
        // put the card on the field if it has a value > 2 or if it is a '1' card and pile isn't bolted is or if it is force
        if( (temp->getValue() > 2 || (temp->getName()=="1" && !isBolted(field__)))
            || (isForce(*temp))) field__.field_.push(*temp);
        switch (temp->getCardType()) {
            case 1: { // bolt, nullify a cards value/effect
                enemyField.field_.top().setBolted(true);
                // if enemy used force, revert it; else subtract the enemy's top card value
                if (isForce(enemyField.field_.top())) enemyField.setPile(enemyField.getPile() / 2);
                else enemyField.setPile(enemyField.getPile() - enemyField.field_.top().getValue());
                break;
            }
            case 2: { // mirror, swaps the fields
                swap(field__, enemyField);
                break;
            }
            case 3: { // blast, shuffles enemy hand and lets you choose which card from that hand to discard
                enemyHand.shuffleHand();
                cout << "Pick a card from your opponent's hand from " << 1 << " to " <<
                     enemyHand.hand_.size() << " to discard." << endl;
                // discard a card from the enemy's Hand
                unsigned int enemyPosition = pickCard(enemyHand);
                discardCard(enemyPosition,enemyHand);
                // sort it back
                enemyHand.sortHand();
                break;
            }
            case 4: { // force, doubles pile
                field__.setPile(field__.getPile() * 2);
                break;
            }
            default: { // regular card
                // if card played is '1' and top card on field is bolted, unbolt it
                if(temp->getName()=="1" && isBolted(field__)) {
                    field__.field_.top().setBolted(false);
                    // if bolted card was force, double pile; else add the bolted card's value back to pile
                    if(isForce(field__.field_.top())) field__.setPile(field__.getPile() * 2);
                    else field__.setPile(field__.getPile() + field__.field_.top().getValue());
                }
                // add value to pile when above condition is false
                else field__.setPile(field__.getPile() + temp->getValue());
                break;
            }
        }
        // if blast was played, discard blast and play another card
        if(temp->getCardType() == 3) {
            discardCard(position,hand__);
            cout << "Now play another card player " << player << "." << endl;
            hand__.printHand();
            playCard(hand__,enemyHand,field__,enemyField,pickCard(hand__));
        }
        // else discard card used from hand
        else discardCard(position,hand__);
    }
} // end playCard

// returns the chosen position of the card from a player's hand
unsigned int duel::pickCard(hand& hand__) {
    // list score of each player
    printScore();
    cout << "Play a card: ";
    // list the player's hand
    hand__.printHand();
    string temp;
    unsigned int position = 0;
    cout << "Pick a card between " << 1 << " and " << hand__.hand_.size() << " ." << endl;
    // repeats if position is out of bounds
    while (!((1 <= position) && (position <= hand__.hand_.size()))) {
        cin >> temp;
        if (is_number(temp)) position = static_cast<unsigned int>(stoi(temp));
        if (!((1 <= position) && (position <= hand__.hand_.size()))) {
            cout << "card must be between " << 1 << " and " << hand__.hand_.size() << " ." << endl;
        }
    }
    cout << "Card chosen: " << hand__.hand_.at(position-1).getName() << endl;
    return (position-1);
} // end pickCard

// check if a player 1 won, player 2 won, or tied
void duel::checkEnd() {
    printScore();
    if(field1.getPile() > field2.getPile()) cout << "Player 1 wins!" << endl;
    else if(field2.getPile() > field1.getPile()) cout << "Player 2 wins!" << endl;
    else cout << "Tie!" << endl;
}

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
} // end createDeck

// discards the card at the position from a hand and subtract that from card type count
void duel::discardCard(unsigned int position, hand& hand__) {
    if (isRegularCard(hand__.hand_.at(position))) hand__.setRegularCards(hand__.getRegularCards()-1);
    else hand__.setSpecialCards(hand__.getSpecialCards()-1);
    hand__.hand_.erase(hand__.hand_.begin() + position);
} // end discardCard

// puts the top card from each deck onto their field
// decks drawn at same rate, so if both decks are empty, then tie
void duel::initializeField() {
    initializeField_();
    player = (field1.getPile() < field2.getPile()) ? 1 : 2;
}

// helper to initializeField(), used for recursion
void duel::initializeField_() {
    // checks if deck empty, then tie
    if(deck.empty()) {
        duelEnd = true;
        return;
    }
    // puts a card from each deck onto the field
    field1.field_.push(deck.top());
    deck.pop();
    field1.setPile(field1.getPile()+field1.field_.top().getValue());
    field2.field_.push(deck.top());
    deck.pop();
    field2.setPile(field2.getPile()+field2.field_.top().getValue());
    // if piles are of equal value then reset field and draw again
    if(equalValue()) {
        resetField();
        initializeField_();
    }
} // end initializeField

// fills both hands with 10 cards and sorts them
void duel::initializeHands() {
    hand1.createHand(deck);
    hand2.createHand(deck);
    hand1.sortHand();
    hand2.sortHand();
} // end initializeHands

// resets both sides of field if piles are equal value
void duel::resetField(){
    field1.resetPile();
    field2.resetPile();
    field1.clearField();
    field2.clearField();
}

// output a deck
void duel::printDeck() {
    while(!deck.empty()){
        cout << deck.top().getName() << endl;
        deck.pop();
    }
} // end printDeck

// output the score
void duel::printScore() {
    cout << "player 1: " << field1.getPile() << '\t' << "player 2: " << field2.getPile() << endl;
} // end printScore

// output both fields top card
void duel::printTopFields() {
    cout << "Field 1 top card: ";
    cout << field1.field_.top().getName() << " ";
    cout << endl;
    cout << "Field 2 top card: ";
    cout << field2.field_.top().getName() << " ";
    cout << endl;
} // end printTopFields

#endif //BLADE_DUEL_H