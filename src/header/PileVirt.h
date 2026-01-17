#ifndef PILEVIRT_H
#define PILEVIRT_H

#include "Types.h"
#include <vector>

class Pile {
private:
    Solitaire::PileType pileT;
public:
    Pile();
    ~Pile();
    virtual Solitaire::PileType getPileT() = 0;
};
#endif // PILEVIRT_H
