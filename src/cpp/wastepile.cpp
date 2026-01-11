#include "../header/wastepile.h"

WastePile::WastePile() : Pile(Solitaire::PileType::Waste) {}

bool WastePile::canAcceptCard(const CardPtr& card) const {
    return card && card->isFaceUp();
}

bool WastePile::canRemoveCard() const {
    return !m_cards.empty();
}

std::shared_ptr<Card> WastePile::takeTopCard() {
    return removeCard();
}

void WastePile::clear() {
    m_cards.clear();
}