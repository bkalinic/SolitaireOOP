#include "../header/deck.h"
#include "../header/tablepile.h"
#include "../header/card.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

Deck::Deck() : m_nextCardId(0) {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    m_rng.seed(seed);
}

void Deck::createFullDeck() {
    clear();
    for (int s = 0; s < 4; ++s) {
        Solitaire::Suit suit = static_cast<Solitaire::Suit>(s);
        for (int r = 1; r <= 13; ++r) {
            Solitaire::Rank rank = static_cast<Solitaire::Rank>(r);
            m_cards.push_back(make_shared<Card>(suit, rank, m_nextCardId++));
        }
    }
}

void Deck::shuffle() {
    std::shuffle(m_cards.begin(), m_cards.end(), m_rng);
}

void Deck::clear() {
    m_cards.clear();
}

CardPtr Deck::drawCard() {
    if (m_cards.empty()) {
        return nullptr;
    }
    CardPtr card = m_cards.back();
    m_cards.pop_back();
    return card;
}

vector<CardPtr> Deck::drawCards(int count) {
    vector<CardPtr> drawn;
    for (int i = 0; i < count && !m_cards.empty(); ++i) {
        drawn.push_back(drawCard());
    }
    return drawn;
}

void Deck::dealToTable(vector<shared_ptr<TablePile>>& tablePiles) {
    if (tablePiles.size() != 7) {
        throw runtime_error("Table mora imati tocno 7 pile-ova");
    }
    for (auto& pile : tablePiles) {
        pile->clear();
    }
    for (int pileIndex = 0; pileIndex < 7; ++pileIndex) {
        for (int i = 0; i <= pileIndex; ++i) {
            if (m_cards.empty()) {
                throw runtime_error("Nema dovoljno karata u decku!");
            }
            CardPtr card = m_cards.back();
            m_cards.pop_back();
            bool faceUp = (i == pileIndex);
            tablePiles[pileIndex]->addCardForInitialDeal(card, faceUp);
        }
    }
}

void Deck::print() const {
    std::cout << "Deck (" << m_cards.size() << " karata):" << std::endl;
    for (size_t i = 0; i < m_cards.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << m_cards[i]->toString() << std::endl;
    }
}