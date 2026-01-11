#ifndef FOUNDATIONPILE_H
#define FOUNDATIONPILE_H

#include "pile.h"
#include "types.h"

class FoundationPile : public Pile {
private:
    Solitaire::Suit m_targetSuit;

public:
    FoundationPile(Solitaire::Suit suit);

    bool canAcceptCard(const CardPtr& card) const override;
    bool canRemoveCard() const override;

    Solitaire::Suit targetSuit() const { return m_targetSuit; }

    bool isComplete() const;

    int nextRequiredValue() const;
};

#endif