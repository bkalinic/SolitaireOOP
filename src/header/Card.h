#ifndef CARD_H
#define CARD_H

#include "Types.h"
#include <iostream>
#include <stdexcept>

class Card {
private:
    Solitaire::Suit cardSuit;
    Solitaire::Rank cardRank;
    bool faceUp;
public:
    Card();
    ~Card();
    Solitaire::Suit getSuit() const;
    Solitaire::Rank getRank() const;
    bool isFaceUp() const;
    void flipCard();
    void setFaceUp();
};

using cardPtr = std::shared_ptr<Card>;
#endif // CARD_H
