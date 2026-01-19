#include "../header/GameEngine.h"
#include <vector>

GameEngine::GameEngine(Deck& d, WastePile& ws, std::vector<TablePile>& tp, std::vector<FoundationPile>& fp)
    :deck(d),waste(ws),table(tp),foundations(fp){}

void GameEngine::gameStart(int pileInd) {
    auto pile = getTable(pileInd).getPileVct();
    if (pile.empty()) {
        pile.back()->flipCard();
    }
}

bool GameEngine::canPick(const Selection& s, const cardPtr& card){
    if(!card->isFaceUp()){
        return false;
    }
    if(s.getArea() == Solitaire::PileType::F){
        return false;
    }
    return true;
}

bool GameEngine::canPlace(const Selection& s, const cardPtr& card){
    if(s.getArea() == Solitaire::PileType::T){
        auto pile = getTable(s.getPileIndex()).getPileVct();
        if(pile.empty()){
            if(card->getRank() == Solitaire::Rank::Kralj){
                return true;
            }
        }
        if(card->isRed() != pile.back()->isRed() && static_cast<int>(card->getRank()) == static_cast<int>(pile.back()->getRank())-1){
            return true;
        }
        return false;
    }
    if(s.getArea() == Solitaire::PileType::F){
        auto pile = getTable(s.getPileIndex()).getPileVct();
        if(pile.empty()){
            if(card->getRank() == Solitaire::Rank::As){
                return true;
            }
        }
        if(pile.back()->getSuit() == card->getSuit() && static_cast<int>(card->getRank()) == static_cast<int>(pile.back()->getSuit())){
            return true;
        }
        return false;
    }
    return false;
}

cardPtr GameEngine::getSelCard(const Selection& s){
    if(s.getArea() == Solitaire::PileType::T){
        auto v = getTable(s.getPileIndex()).getPileVct();
        if(s.getCardIndex() >= 0 && s.getCardIndex() < (int)v.size()){
            return v[s.getCardIndex()];
        }
    }

    if(s.getArea() == Solitaire::PileType::W){
        auto v = getWaste().getWasteVct();
        if(!v.empty()){
            return v.back();
        }
    }

    if(s.getArea() == Solitaire::PileType::F){
        auto v = getFoundations(s.getPileIndex()).getVct();
        if(!v.empty()){
            return v.back();
        }
    }
    return nullptr;
}

bool GameEngine::pick(Selection& s){
    cardPtr card = getSelCard(s);
    if(!card) return false;
    if(!canPick(s,card)) return false;

    s.pickCard();
    return true;
}

void GameEngine::cancel(Selection& s){
    s.cancel();
}

void GameEngine::moveCard(Solitaire::PileType fromType, int fromPile, int fromCard, Solitaire::PileType toType, int toPile){
    std::vector<cardPtr> src;
    std::vector<cardPtr> dst;

    if(fromType == Solitaire::PileType::T){
        src = table[fromPile].getPileVct();
    }else if(fromType == Solitaire::PileType::W){
        src = waste.getWasteVct();
    }

    if(toType == Solitaire::PileType::T){
        dst = table[toPile].getPileVct();
    }else if(toType == Solitaire::PileType::F){
        dst = foundations[toPile].getVct();
    }

    auto it = src.begin() + fromCard;
    dst.insert(dst.end(), it, src.end());
    src.erase(it, src.end());

    if(fromType == Solitaire::PileType::T && !src.empty()){
        src.back()->setFaceUp();
    }
}

bool GameEngine::place(Selection& s){
    auto from = s.cardFrom;
    if(!from) return false;

    cardPtr card = getSelCard(s);
    if(!canPlace(s,card)) return false;

    moveCard(from->type, from->pileI, from->cardI, s.getArea(),s.getPileIndex());

    s.placeCard();
    return true;
}


bool GameEngine::handleEnter(Selection& s){
    if(s.getArea()==Solitaire::PileType::D){
        if(deck.remainingCards()<=0){
            return false;
        }else{
            cardPtr cd = deck.drawCard();
            waste.addToPile(cd);
        }
        return true;
    }
    if (!s.getHold()) {
        return pick(s);
    }
    else{
        return place(s);
    }
}
