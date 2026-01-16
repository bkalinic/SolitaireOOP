#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include "PileVirt.h"
#include <random>
#include <algorithm>
#include <stdexcept>

class Deck : public Pile{
private:
    vector<cardPtr> deckVct;
public:
    Deck();
    ~Deck();
    void createDeck();
    void shuffleDeck();
    size_t remainingCards const();
    cardPtr drawCard();
    inline Solitaire::PileType getPileT(){return Pile::pileT}
};

#endif // DECK_H
