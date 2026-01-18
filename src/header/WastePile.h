#ifndef WASTEPILE_H
#define WASTEPILE_H

#include "PileVirt.h"
#include "Card.h"
#include <vector>

class WastePile : public Pile{
private:
    Solitaire::PileType tType = Solitaire::PileType::F;
    std::vector<cardPtr> wPileVct;
public:
    WastePile();
    ~WastePile();

    std::vector<cardPtr> getWasteVct();

    Solitaire::PileType getPileT() { return tType; }
    void addToPile(cardPtr card);
};
#endif // WASTEPILE_H
