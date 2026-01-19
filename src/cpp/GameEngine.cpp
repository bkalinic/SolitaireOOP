#include "../header/GameEngine.h"

GameEngine::GameEngine(Deck& d, WastePile& ws, std::vector<TablePile>& tp, std::vector<FoundationPile>& fp)
    :deck(d),waste(ws),table(tp),foundations(fp){}

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
        auto& pile = table[s.getPileIndex()].getPileVct();
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
        auto& pile = table[s.getPileIndex()].getVct();
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
        auto& v = table[s.getPileIndex()].getPileVct();
        if(s.getCardIndex() >= 0 && s.getCardIndex() < (int)v.size()){
            return v[s.getCardIndex()];
        }
    }

    if(s.getArea() == Solitaire::PileType::W){
        auto& v = waste->getWasteVct();
        if(!v.empty()){
            return v.back();
        }
    }

    if(s.getArea() == Solitaire::PileType::F){
        auto& v = foundations[s.getPileIndex()].getVct();
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

    s.pick();
    return true;
}

void GameEngine::cancel(Selection& s){
    s.cancel();
}


void GameEngine::moveCard(Solitaire::PileType fromType, int fromPile, int fromCard, Solitaire::PileType toType, int toPile){
    (std::vector<cardptr>)* src = nullptr;
    (std::vector<cardptr>)* dst = nullptr;

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

    auto it = src->begin() + fromCard;
    dst -> insert(dst->end(), it, src->empty());
    src -> erase(it, src->end());

    if(fromType == Solitaire::PileType::T && !src->empty()){
        src->back()->setFaceUp(true);
    }
}

bool GameEngine::place(Selection& s){
    auto from = s.sourceSnap();
    if(!from) return false;

    cardPtr card = getSelCard(*from);
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
    if(!s.holding()){
        return pick(s);
    else{
            return place(s);
        }
    }
}
