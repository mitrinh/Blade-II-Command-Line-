//
// Created by PC on 6/15/2018.
//

#ifndef BLADE_DUEL_H
#define BLADE_DUEL_H

#include "field.h"
#include "hand.h"

#define CARD_TYPES 11 // 11 card types: 1-7, bolt, mirror, blast, force
#define CARD_QUANTITY_MAX 4


class duel {
private:
    bool duelEnd; // shows when duel is over
    bool equalValue();
    const unsigned int totalCards = CARD_TYPES*CARD_QUANTITY_MAX;
    unsigned int player; // 1 = player 1 , 2 = player 2 , ...
    unsigned end; // 0 = tie , 1 = p1 win, 2 = p2 win
    stack<card> deck;
    field field1;
    field field2;
    hand hand1;
    hand hand2;
    void checkEnd();
    void createDeck(const card[]);
    void initializeField();
public:
    explicit duel(card[]);
    ~duel();
    void playCard(hand&, hand&, field&, field&, unsigned int);
    void printDeck();
}; // end class duel

// checks if the top card is bolted
bool isBolted(const field &field__) { return (field__.field_.top().getBolted()); }

// checks if a card is force
bool isForce(const card &card_) { return card_.getCardType() == 4; }

duel::duel(card cards[]) {
    player = 1;
    end = 0;
    duelEnd = false;
    while(!deck.empty()) deck.pop();
    createDeck(cards);
    hand1.createHand(deck);
    hand2.createHand(deck);
    hand1.sortHand();
    hand2.sortHand();
    hand1.printHand();
    hand2.printHand();
    initializeField();
    cout << "player 1: " << field1.getPile() << endl;
    cout << "player 2: " << field2.getPile() << endl << endl;
    while(!duelEnd) {
        if(player == 1) {
            cout << "\tplayer " << player << "'s turn.\n" << "Play a card" << endl;
            hand1.printHand();
            playCard(hand1,hand2,field1,field2,0);
        }
        else {
            cout << "\tplayer " << player << "'s turn.\n" << "Play a card" << endl;
            hand2.printHand();
            playCard(hand2,hand1,field2,field1,0);
        }
    }
    cout << "player 1: " << field1.getPile() << endl;
    cout << "player 2: " << field2.getPile() << endl;
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
            enemyHand.shuffleHand();
            cout << "Pick a card from your opponent's hand from " << 1 << " to " <<
                 enemyHand.hand_.size() << " to discard.";
            unique_ptr <int> position_(new int);
            // repeats if position is out of bounds
            *position_ = 0;
            while (!((1 <= *position_) && (*position_ < enemyHand.hand_.size()))) {
                cin >> *position_;
                if ((1 <= *position_) && (*position_ < enemyHand.hand_.size()))
                    cout << "card must be between" << 1 << " and " << enemyHand.hand_.size();
            }
            // discard that card
            enemyHand.hand_.erase(enemyHand.hand_.begin() + *position_ - 1);
            // sort it back
            enemyHand.sortHand();
            break;
        }
        case 4: { // force
            field__.setPile(field__.getPile() * 2);
            break;
        }
        default: { // regular card
            if(temp->getValue()==1 && isBolted(field__)) {
                field__.field_.top().setBolted(false);
                if(isForce(field__.field_.top())) field__.setPile(field__.getPile() * 2);
                else field__.setPile(field__.getPile() + field__.field_.top().getValue());
            }
            field__.setPile(field__.getPile() + temp->getValue());
            break;
        }
    }
    if(isRegularCard(*temp)) hand__.setRegularCards(--hand__.getRegularCards());
    else hand__.setSpecialCards(--hand__.getSpecialCards());
    // discard card used from hand
    hand__.hand_.erase(hand__.hand_.begin() + position);
} // end playCard

// check if a player 1 won, player 2 won, or tied
void duel::checkEnd() {
    if(field1.getPile() > field2.getPile()) end = 1;
    else if(field2.getPile() > field1.getPile()) end = 2;
    else end = 0;
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
} // end duel

// output a deck
void duel::printDeck() {
    while(!deck.empty()){
        cout << deck.top().getName() << endl;
        deck.pop();
    }
} // end printDeck

// puts the top card from each deck onto their field
// decks drawn at same rate, so if both decks are empty, then tie
void duel::initializeField() {
    while(equalValue()){
        // checks if deck empty, then tie
        if(deck.empty()) {
            end = 2;
            duelEnd = true;
            break;
        }
        field1.field_.push(deck.top());
        deck.pop();
        field1.setPile(field1.getPile()+field1.field_.top().getValue());
        field2.field_.push(deck.top());
        deck.pop();
        field2.setPile(field2.getPile()+field2.field_.top().getValue());
        // if piles are of equal value then reset field and draw again
        if(equalValue()) {
            field1.setPile(0);
            field2.setPile(0);
            while(!field1.field_.empty()) field1.field_.pop();
            while(!field2.field_.empty()) field2.field_.pop();
        }
        // else, player 1 goes first if pile is less than player 2, else player 2's turn
        else player = (field1.getPile() < field2.getPile()) ? 1 : 2;
    }
} // end initializeField

// returns whether both piles are of equal value or not
bool duel::equalValue() { return field1.getPile() == field2.getPile();}

#endif //BLADE_DUEL_H