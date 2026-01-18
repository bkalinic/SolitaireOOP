#include "../header/WastePile.h"

WastePile::WastePile() {}

WastePile::~WastePile(){}

std::vector<cardPtr> WastePile::getWasteVct() {
	return wPileVct;
}

void WastePile::addToPile(cardPtr card) {
	wPileVct.push_back(card);
}