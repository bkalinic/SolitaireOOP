#ifndef TABLEPILE_H
#define TABLEPILE_H

#include "PileVirt.h"
#include "Card.h"
#include "Deck.h"
#include <vector>

class TablePile : public Pile {
private:
    Solitaire::PileType tType = Solitaire::PileType::T;
    std::vector<cardPtr> tPileVct;
public:
    TablePile(Deck& deck, int n);
    ~TablePile();

    std::vector<cardPtr> getPileVct();

    void generatePile(Deck& deck, int n);
    Solitaire::PileType getPileT() override { return tType; }
    inline size_t getPileSize() const {return tPileVct.size()};
};
#endif // TABLEPILE_H
