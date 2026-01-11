#include "../header/card.h"
#include <iostream>
#include <ostream>
#include <string>

Card::Card(Solitaire::Suit suit, Solitaire::Rank rank, int id)
    : m_suit(suit), m_rank(rank), m_faceUp(false), m_id(id) {
}

bool Card::isRed() const {
    return m_suit == Solitaire::Suit::Herc || m_suit == Solitaire::Suit::Karo;
}

int Card::value() const {
    return static_cast<int>(m_rank);
}

void Card::flip() {
    m_faceUp = !m_faceUp;
}

void Card::setFaceUp(bool faceUp) {
    m_faceUp = faceUp;
}

bool Card::canPlaceOn(const Card* other) const {
    if (!other || !m_faceUp) {
        return false;
    }
    return (this->isRed() != other->isRed()) && (this->value() == other->value() - 1);
}

std::string Card::toString() const {
    return rankToString(m_rank) + " " + suitToString(m_suit) + " [" + (m_faceUp ? "lice gore" : "lice dolje") + "]";
}

std::string Card::shortString() const {
    std::string rankStr;
    switch (m_rank) {
    case Solitaire::Rank::As:    rankStr = "A"; break;
    case Solitaire::Rank::Jack:  rankStr = "J"; break;
    case Solitaire::Rank::Dama:  rankStr = "Q"; break;
    case Solitaire::Rank::Kralj: rankStr = "K"; break;
    default: rankStr = std::to_string(value());
    }

    return rankStr + (m_faceUp ? " [otkriven]" : " [okrenut]");
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << card.shortString();
    return os;
}