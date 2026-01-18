#include "../header/GameEngine.h"

void drawFromStockToWaste(Deck& deck, WastePile& waste) {
	cardPtr drawn = deck.drawCard();
	waste.addToPile(drawn);
}