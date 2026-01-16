#ifndef PILEVIRT_H
#define PILEVIRT_H
#include "Types.h"
#include "Card.h"
#include <vector>
class Pile{
private:
    Solitaire::PileType pileT;
public:
    Pile(Solitaire::PileType pT);
    ~Pile();
    virtual inline Solitaire::PileType getPileT = 0;
};
#endif // PILEVIRT_H
