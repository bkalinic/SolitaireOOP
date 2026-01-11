#include "../header/tablepile.h"
#include <vector>

TablePile::TablePile() : Pile(Solitaire::PileType::Table) {}

void TablePile::addCardForInitialDeal(CardPtr card, bool isFaceUp) {
    card->setFaceUp(isFaceUp);
    m_cards.push_back(card);
}

bool TablePile::canAcceptCard(const CardPtr& card) const {
    if (!card || !card->isFaceUp()) {
        return false;
    }
    if (m_cards.empty()) {
        return card->value() == 13;
    }
    CardPtr top = topCard();
    return card->canPlaceOn(top.get());
}

bool TablePile::canAcceptCards(const vector<CardPtr>& cards) const {
    if (cards.empty()) return false;

    if (!canAcceptCard(cards.front())) {
        return false;
    }
    for (size_t i = 0; i < cards.size() - 1; ++i) {
        if (!cards[i]->canPlaceOn(cards[i + 1].get())) {
            return false;
        }
    }
    return true;
}

bool TablePile::canRemoveCard() const {
    return !m_cards.empty() && topCard()->isFaceUp();
}

bool TablePile::canDragFrom(int index) const {
    if (index < 0 || index >= static_cast<int>(m_cards.size())) {
        return false;
    }
    for (int i = index; i < static_cast<int>(m_cards.size()); ++i) {
        if (!m_cards[i]->isFaceUp()) {
            return false;
        }
    }
    return isValidSequenceFrom(index);
}

void TablePile::flipTopCard() {
    if (!m_cards.empty() && !topCard()->isFaceUp()) {
        topCard()->flip();
    }
}

bool TablePile::isValidSequenceFrom(int index) const {
    if (index < 0 || index >= static_cast<int>(m_cards.size()) - 1) {
        return true;
    }
    for (int i = index; i < static_cast<int>(m_cards.size()) - 1; ++i) {
        const Card* current = m_cards[i].get();
        const Card* next = m_cards[i + 1].get();

        if (!current->canPlaceOn(next)) {
            return false;
        }
    }
    return true;
}
