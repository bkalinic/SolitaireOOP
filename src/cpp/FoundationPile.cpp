#include "../header/FoundationPile.h"

FoundationPile::FoundationPile(Solitaire::Suit s) : fPileSuit(s) {}

FoundationPile::~FoundationPile(){}

std::vector<cardPtr> FoundationPile::getVct() {
	return fPileVct;
}