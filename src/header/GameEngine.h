#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Card.h"
#include "Deck.h"
#include "WastePile.h"

void drawFromStockToWaste(Deck& deck, WastePile& waste);

#endif