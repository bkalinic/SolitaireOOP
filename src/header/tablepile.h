#ifndef TABLEPILE_H
#define TABLEPILE_H

#include "pile.h"
#include <vector>

using namespace std;

class TablePile : public Pile {
public:
    TablePile();

    void addCardForInitialDeal(CardPtr card, bool isFaceUp);

    bool canAcceptCard(const CardPtr& card) const override;
    bool canAcceptCards(const vector<CardPtr>& cards) const override;
    bool canRemoveCard() const override;

    bool canDragFrom(int index) const;
    void flipTopCard();
private:
    bool isValidSequenceFrom(int index) const;
};

#endif