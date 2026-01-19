#include "../header/Card.h"
#include "../header/Types.h"

Card::Card(const Solitaire::Suit s, const Solitaire::Rank r, bool fU)
    :cardSuit(s), cardRank(r), faceUp(fU) {}

Card::~Card() {}

Solitaire::Suit Card::getSuit() const {
    return cardSuit;
}

Solitaire::Rank Card::getRank() const {
    return cardRank;
}

bool Card::isFaceUp() const {
    return faceUp;
}

void Card::flipCard() {
    faceUp=!faceUp;
}

void Card::setFaceUp() {
    if(!faceUp){
        flipCard();
    }
}

bool Card::isRed() const {
    if(getSuit()==Solitaire::Suit::H || getSuit()==Solitaire::Suit::K){
        return true;
    }else{
        return false;
    }
}

std::string Card::getSymbol() const {
    if (getSuit() == Solitaire::Suit::H) return "♥";
    if (getSuit() == Solitaire::Suit::K) return "♦";
    if (getSuit() == Solitaire::Suit::T) return "♣";
    if (getSuit() == Solitaire::Suit::P) return "♠";
    return "?";
}

ftxui::Color Card::getColor() const {
    return (getSuit() == Solitaire::Suit::H || getSuit() == Solitaire::Suit::K)
        ? ftxui::Color::Red
        : ftxui::Color::Black;
}

std::ostream& operator<< (std::ostream& os, const Card& card) {
    return os << stringSuit(card.getSuit()) << " " << stringRank(card.getRank()) << ", karta okrenuta: "
            << (card.isFaceUp() == true ? "da" : "ne");
}
