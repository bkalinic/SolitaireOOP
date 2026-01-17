#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include "PileVirt.h"
#include <random>
#include <algorithm>
#include <stdexcept>
#include <vector>

class Deck : public Pile {
private:
    Solitaire::PileType tType = Solitaire::PileType::W;
    std::vector<cardPtr> deckVct;
public:
    Deck();
    ~Deck();
    void createDeck();
    void shuffleDeck();
    size_t remainingCards() const;
    cardPtr drawCard();
    void printDeck() const;
    Solitaire::PileType getPileT() { return tType; }
};

#endif // DECK_H
