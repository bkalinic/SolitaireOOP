// SolitaireFixed.h : Include file for standard system include files,
// or project specific include files.

#ifndef SOLITAIREFIXED_H
#define SOLITAIREFIXED_H

#include <ftxui/dom/elements.hpp>
#include "src/header/Card.h"

using namespace ftxui;

Element CreateCardElement(const Card& card);
Element CreateEmptyPile();
Element CreateFoundationPile(const Card& top_card, int index);

#endif