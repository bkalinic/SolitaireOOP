#ifndef PILEVIRT_H
#define PILEVIRT_H
#include "Types.h"
class Pile{
private:
    Solitaire::PileType pileT;
public:
    Pile();
    ~Pile();
    virtual inline Solitaire::PileType getPileT{return pileT;}
};
#endif // PILEVIRT_H
