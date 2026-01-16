#ifndef TABLEPILE_H
#define TABLEPILE_H
#include "PileVirt.h"

class TablePile : public Pile{
private:
    int tPid;
    vector<cardPtr> tPileVct;
public:
    TablePile(int id);
    ~TablePile();
    inline Solitaire::PileType getPileT(){return Pile::pileT}
};
#endif // TABLEPILE_H
