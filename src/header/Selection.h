#ifndef SELECTION_H
#define SELECTION_H

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "Types.h"
#include "TablePile.h"

using namespace ftxui;
using namespace Solitaire;

class Selection {
private:
	Solitaire::PileType area = Solitaire::PileType::T;
	int pileIndex = 0;
	int cardIndex = 0;
	bool holding = false;
	struct sourceSnap{
        Solitaire::PileType type;
        int pileI;
        int cardI;
	};
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
    void placeCard();

    bool isSelected(Solitaire::PileType t, int pile = 0, int card = 0) const {}
    bool isSource(Solitaire::PileType t, int pile = 0, int card = 0) const {}
};

#endif
