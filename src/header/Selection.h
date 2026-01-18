#ifndef SELECTION_H
#define SELECTION_H

#include "Types.h"
#include "TablePile.h"

class Selection {
private:
	Solitaire::PileType area=Solitaire::PileType::T;
	int pileIndex = 0;
	int cardIndex = 0;
	bool holding = false;
public:
    inline int getPileIndex() const {return pileIndex};
    inline void setPileIndex(int pI) {pileIndex = pI};
    inline int getCardIndex() const {return cardIndex};
    inline void setCardIndex(int cI) const {cardIndex = cI};
    inline Solitaire::PileType getArea() const {return area};
    inline void setArea(Solitaire::PileType pT) {area=pT};
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void pickCard();
};

#endif
