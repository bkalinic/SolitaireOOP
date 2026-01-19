#ifndef FOUNDATIONPILE_H
#define FOUNDATIONPILE_H

#include "PileVirt.h"
#include "Card.h"
#include <vector>

class FoundationPile : public Pile {
private:
    Solitaire::PileType tType = Solitaire::PileType::F;
    Solitaire::Suit fPileSuit;
    std::vector<cardPtr> fPileVct;
    bool locked;
public:
    FoundationPile(Solitaire::Suit fPileSuit);
    ~FoundationPile();
    Solitaire::PileType getPileT() { return tType; }
    std::vector<cardPtr> getVct();
};
#endif // FOUNDATIONPILE_H
