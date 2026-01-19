// SolitaireFixed.h : Include file for standard system include files,
// or project specific include files.

#ifndef SOLITAIREFIXED_H
#define SOLITAIREFIXED_H

#include <ftxui/dom/elements.hpp>
#include "src/header/Card.h"

using namespace ftxui;

Element CreateCardElement(const Card& card, Solitaire::PileType type, int pileIter=0, int cardInPile=0);
Element CreateEmptyPile(Solitaire::PileType type, int pileIter=0, int cardInPile=0);
Element CreateFoundationPile(const cardPtr top_card, Solitaire::Suit suit, Solitaire::PileType type, int pileIter=0);

#endif
