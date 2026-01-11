#include "../header/foundationpile.h"

FoundationPile::FoundationPile(Solitaire::Suit suit) 
    : Pile(Solitaire::PileType::Foundation), m_targetSuit(suit) {}

bool FoundationPile::canAcceptCard(const CardPtr& card) const {
    if (!card || !card->isFaceUp()) {
        return false;
    }
    if (card->suit() != m_targetSuit) {
        return false;
    }
    if (m_cards.empty()) {
        return card->value() == 1;
    }
    CardPtr top = topCard();
    return card->value() == top->value() + 1;
}

bool FoundationPile::canRemoveCard() const {
    return false;
}

bool FoundationPile::isComplete() const {
    return m_cards.size() == 13;
}

int FoundationPile::nextRequiredValue() const {
    if (m_cards.empty()) {
        return 1;
    }
    return topCard()->value() + 1;
}