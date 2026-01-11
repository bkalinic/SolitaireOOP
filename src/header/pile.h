#ifndef PILE_H
#define PILE_H

#include "card.h"
#include "types.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

class Pile {
protected:
    std::vector<CardPtr> m_cards;
    Solitaire::PileType m_type;

public:
    Pile(Solitaire::PileType type);
    virtual ~Pile() = default;

    virtual bool canAcceptCard(const CardPtr& card) const = 0;
    virtual bool canAcceptCards(const std::vector<CardPtr>& cards) const;
    virtual bool canRemoveCard() const = 0;

    bool isEmpty() const { return m_cards.empty(); }
    size_t count() const { return m_cards.size(); }

    CardPtr topCard() const;
    CardPtr cardAt(int index) const;
    const std::vector<CardPtr>& cards() const { return m_cards; }
    Solitaire::PileType type() const { return m_type; }

    virtual void addCard(CardPtr card);
    virtual bool removeSpecificCard(std::shared_ptr<Card> card);
    virtual CardPtr removeCard();
    virtual CardPtr removeTopCard();
    virtual std::vector<CardPtr> removeCardsFrom(int index);

    void clear();

    void print() const;
    std::string toString() const;

    Pile(const Pile&) = delete;
    Pile& operator=(const Pile&) = delete;
};

#endif