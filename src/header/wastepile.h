#ifndef WASTEPILE_H
#define WASTEPILE_H

#include "pile.h"

class WastePile : public Pile {
public:
    WastePile();

    bool canAcceptCard(const CardPtr& card) const override;
    bool canRemoveCard() const override;

    std::shared_ptr<Card> takeTopCard();
    void clear();
};

#endif