#ifndef DECK_H
#define DECK_H

#include "card.h"
#include "tablepile.h"
#include <vector>
#include <memory>
#include <random>

class Deck {
private:
    std::vector<CardPtr> m_cards;
    int m_nextCardId;

public:
    Deck();

    void createFullDeck();
    void shuffle();
    void clear();

    bool isEmpty() const { return m_cards.empty(); }
    size_t remainingCards() const { return m_cards.size(); }
    const vector<CardPtr>& cards() const { return m_cards; }

    CardPtr drawCard();
    vector<CardPtr> drawCards(int count);

    void dealToTable(vector<shared_ptr<TablePile>>& tablePiles);

    void print() const;

private:
    std::mt19937 m_rng;
};

#endif