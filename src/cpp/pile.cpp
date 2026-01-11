#include "../header/pile.h"
#include "../header/types.h"
#include <iostream>
#include <sstream>
#include <vector>

Pile::Pile(Solitaire::PileType type) : m_type(type) {}

bool Pile::canAcceptCards(const std::vector<CardPtr>& cards) const {
    for (const auto& card : cards) {
        if (!canAcceptCard(card)) {
            return false;
        }
    }
    return true;
}

CardPtr Pile::topCard() const {
    if (m_cards.empty()) return nullptr;
    return m_cards.back();
}

CardPtr Pile::cardAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_cards.size())) {
        return nullptr;
    }
    return m_cards[index];
}

void Pile::addCard(CardPtr card) {
    if (card && canAcceptCard(card)) {
        m_cards.push_back(card);
    }
}

bool Pile::removeSpecificCard(std::shared_ptr<Card> card) {
    if (!card) return false;

    auto it = std::find(m_cards.begin(), m_cards.end(), card);
    if (it != m_cards.end()) {
        m_cards.erase(it);
        return true;
    }

    return false;
}

CardPtr Pile::removeCard() {
    if (m_cards.empty()) return nullptr;

    CardPtr card = m_cards.back();
    m_cards.pop_back();
    return card;
}

CardPtr Pile::removeTopCard() {
    if (m_cards.empty()) {
        return nullptr;
    }
    auto card = m_cards.back();
    m_cards.pop_back();

    if (!m_cards.empty() && m_type == Solitaire::PileType::Table) {
        m_cards.back()->setFaceUp(true);
    }
    return card;
}

std::vector<CardPtr> Pile::removeCardsFrom(int index) {
    if (index < 0 || index >= static_cast<int>(m_cards.size())) {
        return {};
    }
    std::vector<CardPtr> removedCards(m_cards.begin() + index, m_cards.end());

    m_cards.erase(m_cards.begin() + index, m_cards.end());
    return removedCards;
}

void Pile::clear() {
    m_cards.clear();
}

void Pile::print() const {
    std::cout << toString() << std::endl;
}

std::string Pile::toString() const {
    std::stringstream ss;
    switch (m_type) {
    case Solitaire::PileType::Table:      ss << "Table"; break;
    case Solitaire::PileType::Foundation: ss << "Foundation"; break;
    case Solitaire::PileType::Stock:      ss << "Stock"; break;
    case Solitaire::PileType::Waste:      ss << "Waste"; break;
    }
    ss << " Pile (" << m_cards.size() << " karata): ";

    if (m_cards.empty()) {
        ss << "[Prazno]";
    }
    else {
        for (size_t i = 0; i < m_cards.size(); ++i) {
            if (i > 0) ss << ", ";
            ss << m_cards[i]->shortString();
        }
    }
    return ss.str();
}