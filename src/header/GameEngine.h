#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Card.h"
#include "Deck.h"
#include "WastePile.h"

class GameEngine {
private:
    Deck& deck;
    WastePile& waste;
    std::vector<TablePile>& table;
    std::vector<FoundationPile>& foundations;
public:
    GameEngine(Deck& d, WastePile& ws, std::vector<TablePile>& tp, std::vector<FoundationPile>& fp);
    bool handleEnter(Selection& s);
    void cancel(Selection& s);
    bool pick(Selection& s);
    bool place(Selection& s);
    cardPtr getSelCard(const Selection& s);
    bool canPick(const Selection& s, const cardPtr& card);
    bool canPlace(const Selection& s, const cardPtr& card);
    void moveCard(Solitaire::PileType fromType, int fromPile, int fromCard, Solitaire::PileType toType, int toPile);
}

#endif
