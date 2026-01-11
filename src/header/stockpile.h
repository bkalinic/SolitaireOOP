#ifndef STOCKPILE_H
#define STOCKPILE_H

#include "pile.h"
#include <vector>

class StockPile : public Pile {
public:
    StockPile();

    bool canAcceptCard(const CardPtr& card) const override;
    bool canRemoveCard() const override;

    void resetFromWaste(const std::vector<CardPtr>& wasteCards);
    std::vector<std::shared_ptr<Card>> drawCards(int count);
};

#endif