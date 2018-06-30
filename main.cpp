//
// Created by PC on 6/15/2018.
//

#include "duel.h"

/* generates each type of card */
void generateCards(card cards[]) {
    card *temp;
    for (int i = 0; i < CARD_TYPES; i++) {
        temp = new card();
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
                    break;
                case 8:
                    temp->setName("mirror");
                    break;
                case 9:
                    temp->setName("blast");
                    break;
                case 10:
                    temp->setName("force");
                    break;
                default:
                    cout << "An error has occurred. " << "cards cannot be generated";
            }
        }
        cards[i] = *temp;
        delete temp;
    }
}

int main() {
    card cards[CARD_TYPES];
    generateCards(cards);
    duel d(cards);
    return 0;
}