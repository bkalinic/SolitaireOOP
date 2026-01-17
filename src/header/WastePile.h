#ifndef WASTEPILE_H
#define WASTEPILE_H

#include "PileVirt.h"
#include <vector>

class WastePile : public Pile{
private:
    Solitaire::PileType tType = Solitaire::PileType::F;
    std::vector<cardPtr> wPileVct;
public:
    WastePile();
    ~WastePile();
    Solitaire::PileType getPileT() { return tType; }
};
#endif // WASTEPILE_H
