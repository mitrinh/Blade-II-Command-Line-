//
//    Name:         Trinh, Michael
//    Project:      Blade
//    Date Started: 06/15/18
//    Date Finished:08/21/18
//
//    Description:      A card game based on the minigame in the "Trails" video game franchise.
//                  -The game is set up with a deck of 44 cards and is shared between 2 players holding 10 cards taken
//                      10 cards from the deck.
//                  -The cards have their own value or score whenever they are played.
//                  -At the start of the game, both players play a card from the top of the deck and whoever's score is
//                      lower goes first.
//                  -The win condition is to "checkmate" the other  player so that their score is lower than yours after
//                      they play a card.
//                  -Whenever both scores are equal,clear all played cards and current scores then start the game again.
//                  -There are 2 types of cards: regular cards of values 1-7, and special cards which have a base value
//                      of 1 when played from the deck or an effect depending on the card when played from the hand.
//
//                  -There are 4 different special cards: blast, bolt, force, and mirror.
//                      -blast lets you choose a random card from your opponent's hand and discard, then lets you play
//                          again.
//                      -bolt negates the opponent's last played card's value if it is regular or force. Cards that have
//                          been bolted can be restored by a regular "1" card.
//                      -force doubles the score of your played cards.
//                      -mirror swaps the opponent's played cards and score with yours.

#include "duel.h"

// generates each type of card
void generateCards(card cards[]);

// prompts the user to answer the yes or no question, returns true for yes, false for no
bool askYesOrNo(const string &question);

// generates each type of card
void generateCards(card cards[]) {
    unique_ptr<card>temp(new card);
    for (unsigned int i = 0; i < CARD_TYPES; i++) {
        // numbered cards 1-7
        if (i < 7) {
            temp->setValue(i+1);
            temp->setName(to_string(i+1));
        }
        // special cards
        else {
            temp->setValue(1); // can be used to check if a card is numbered or special
            switch(i) {
                case 7:
                    temp->setName("bolt");
                    temp->setCardType(1);
                    break;
                case 8:
                    temp->setName("mirror");
                    temp->setCardType(2);
                    break;
                case 9:
                    temp->setName("blast");
                    temp->setCardType(3);
                    break;
                case 10:
                    temp->setName("force");
                    temp->setCardType(4);
                    break;
                default: cout << "An error has occurred. " << "cards cannot be generated";
            }
        }
        cards[i] = *temp;
    }
} // end generateCards

// prompts the user to answer the yes or no question, returns true for yes, false for no
bool askYesOrNo(const string &question){
    printf("The score is %d - %d.\n",player1Score,player2Score);
    cout << question << endl;
    string answer = "abc";
    while(answer.at(0) != 'y' && answer.at(0) != 'n') {
        cin >> answer;
        if(answer.at(0) != 'y' && answer.at(0) != 'n') cout << "Type y or n." << endl;
    }
    return answer.at(0) == 'y';
} // end askYesOrNo

// main
int main() {
    card cards[CARD_TYPES];
    generateCards(cards);
    if(askYesOrNo("Do you want to duel? y for yes, n for no")) {
        duel* d = new duel(cards);
        delete d;
        while(askYesOrNo("Do you want to rematch? y for yes, n for no")) {
            d = new duel(cards);
            delete d;
        }
    }
    cout << "Goodbye!" << endl;
    return 0;
} // end main