#include "../header/stockpile.h"
#include <vector>

StockPile::StockPile() : Pile(Solitaire::PileType::Stock) {}

bool StockPile::canAcceptCard(const CardPtr& card) const {
    return card && !card->isFaceUp();
}

bool StockPile::canRemoveCard() const {
    return !m_cards.empty();
}

void StockPile::resetFromWaste(const std::vector<CardPtr>& wasteCards) {
    clear();
    for (const auto& card : wasteCards) {
        card->setFaceUp(false);
        addCard(card);
    }
}

std::vector<std::shared_ptr<Card>> StockPile::drawCards(int count) {
    std::vector<std::shared_ptr<Card>> drawn;
    for (int i = 0; i < count && !isEmpty(); ++i) {
        auto card = removeCard();
        if (card) {
            card->setFaceUp(true);
            drawn.push_back(card);
        }
    }
    return drawn;
}