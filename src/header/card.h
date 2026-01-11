#ifndef CARD_H
#define CARD_H

#include "types.h"
#include <memory>
#include <string>

class Card {
private:
    Solitaire::Suit m_suit;
    Solitaire::Rank m_rank;
    bool m_faceUp;
    int m_id;

public:
    Card(Solitaire::Suit suit, Solitaire::Rank rank, int id = 0);

    Solitaire::Suit suit() const { return m_suit; }
    Solitaire::Rank rank() const { return m_rank; }
    bool isFaceUp() const { return m_faceUp; }
    int id() const { return m_id; }

    bool isRed() const;
    bool isBlack() const { return !isRed(); }

    int value() const;

    void flip();
    void setFaceUp(bool faceUp);

    bool canPlaceOn(const Card* other) const;

    std::string toString() const;
    std::string shortString() const;

    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};

using CardPtr = std::shared_ptr<Card>;

#endif