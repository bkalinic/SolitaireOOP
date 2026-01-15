#include "../header/Card.h"

Card::Card(Solitaire::Suit s, Solitaire::Rank r, bool fU)
    :cardSuit(s), cardRank(r), faceUp(fU){}

Card::~Card(){}

Solitaire::Suit getSuit() const{
    return cardSuit;
}

Solitaire::Rank getRank() const{
    return cardRank;
}

bool isFaceUp() const{
    return faceUp;
}

void flipCard() {
    faceUp=!faceUp;
}

void setFaceUp() {
    if(!faceUp){
        flipCard();
    }
}
