#ifndef WASTEPILE_H
#define WASTEPILE_H
#include "PileVirt.h"

class WastePile : public Pile{
private:
    vector<cardPtr> wPileVct;
public:
    WastePile();
    ~WastePile();
    inline Solitaire::PileType getPileT(){return Pile::pileT}
};
#endif // WASTEPILE_H
