#ifndef SELECTION_H
#define SELECTION_H

#include "Types.h"
#include "TablePile.h"

class Selection {
private:
	Solitaire::PileType area = Solitaire::PileType::T;
	int pileIndex = 0;
	int cardIndex = 0;
	bool holding = false;
public:
    Selection();
    ~Selection();

    int getPileIndex() const { return pileIndex; }
    void setPileIndex(int pI) { pileIndex = pI; }
    int getCardIndex() const { return cardIndex; }
    void setCardIndex(int cI) { cardIndex = cI; }
    Solitaire::PileType getArea() const { return area; }
    void setArea(Solitaire::PileType pT) { area = pT; }

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void pickCard();
};

#endif
