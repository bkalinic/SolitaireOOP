#ifndef FOUNDATIONPILE_H
#define FOUNDATIONPILE_H
#include "PileVirt.h"

class FoundationPile : public Pile{
private:
    Solitaire::Suit fPileSuit;
    vector<cardPtr> fPileVct;
public:
    FoundationPile(Solitaire::Suit fPileSuit);
    ~FoundationPile();
    inline Solitaire::PileType getPileT(){return Pile::pileT}
};
#endif // FOUNDATIONPILE_H
