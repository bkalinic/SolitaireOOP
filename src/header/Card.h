#ifndef CARD_H
#define CARD_H

#include <ftxui/screen/color.hpp>
#include "Types.h"
#include <iostream>
#include <stdexcept>

class Card {
private:
    Solitaire::Suit cardSuit;
    Solitaire::Rank cardRank;
    bool faceUp = false;
public:
    Card(const Solitaire::Suit s, const Solitaire::Rank r, bool fU);
    ~Card();
    Solitaire::Suit getSuit() const;
    Solitaire::Rank getRank() const;
    bool isFaceUp() const;
    void flipCard();
    void setFaceUp();

    std::string getSymbol() const;
    ftxui::Color getColor() const;
};

std::ostream& operator<< (std::ostream& os, const Card& card);
using cardPtr = std::shared_ptr<Card>;
#endif // CARD_H
