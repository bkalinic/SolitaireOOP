#include "../header/TablePile.h"
#include "../header/Deck.h"

TablePile::TablePile(Deck& deck, int n) {
	generatePile(deck, n);
}

TablePile::~TablePile(){}

void TablePile::generatePile(Deck& deck, int n) {
	for (int i = 0; i < n; i++) {
		tPileVct.push_back(deck.drawCard());
	}
}