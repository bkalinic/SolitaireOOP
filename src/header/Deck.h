#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <random>

class Deck{
private:
    vector<cardPtr> cards;
public:
    Deck();
    ~Deck();
    void createDeck();
    void shuffle();
    bool isEmpty();
    size_t remainingCards const();
    cardPtr drawCard();

};

#endif // DECK_H
