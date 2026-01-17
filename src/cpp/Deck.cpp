#include "../header/Deck.h"
#include "../header/Card.h"
#include "../header/Types.h"
#include <iostream>
#include <algorithm>

Deck::Deck() {
    createDeck();
}

Deck::~Deck() {}

void Deck::createDeck() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            cardPtr tmp = std::make_shared<Card>(i, j, false);
            deckVct.push_back(tmp);
        }
    }
}

void Deck::shuffleDeck(){
    auto rng = std::default_random_engine{};
    std::shuffle(deckVct.begin(), deckVct.end(), rng);
}

size_t Deck::remainingCards() const {
    return deckVct.size();
}

cardPtr Deck::drawCard() {
    cardPtr returning = deckVct.back();
    deckVct.pop_back();
    return returning;
}

void Deck::printDeck() const {
    for (int i = 0; i < 52; i++) {
        std::cout << deckVct[i] << std::endl;
    }
}