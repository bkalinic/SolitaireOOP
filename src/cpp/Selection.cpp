#include "../header/Selection.h"
#include "../header/Types.h"

Selection::Selection() = default;

Selection::~Selection() {}

void Selection::moveRight(){
    switch(getArea()){
    case Solitaire::PileType::D:
        setArea(Solitaire::PileType::W);
        break;
    case Solitaire::PileType::W:
        setArea(Solitaire::PileType::F);
        setPileIndex(0);
        break;
    case Solitaire::PileType::F:
        if (getPileIndex() > 3) {
            setArea(Solitaire::PileType::T);
            setPileIndex(0);
            setCardIndex(0);
        }
        break;
    case Solitaire::PileType::T:
        if (getPileIndex() < 6) {
            setPileIndex(getPileIndex() + 1);
            break;
        }
    }
}

void Selection::moveLeft() {
    switch(getArea()){
    case Solitaire::PileType::T:
        if (getPileIndex() == 0) {
            setArea(Solitaire::PileType::F);
            setPileIndex(3);
        }else{
            setPileIndex(getPileIndex() - 1);
        }
        break;
    case Solitaire::PileType::F:
        if (getPileIndex() == 0) {
            setArea(Solitaire::PileType::W);
        }else{
            setPileIndex(getPileIndex() - 1);
        }
        break;
    case Solitaire::PileType::W:
        setArea(Solitaire::PileType::D);
        break;
    default: break;
    }
}

void Selection::moveUp() {
    if (getArea() == Solitaire::PileType::T && getCardIndex() > 0) {
        setCardIndex(getCardIndex() - 1);
    }
    else if (getCardIndex() == 0){
        switch (getPileIndex()) {
        case(0):
            setArea(Solitaire::PileType::D);
            break;
        case(1):
            setArea(Solitaire::PileType::D);
            break;
        case(2):
            setArea(Solitaire::PileType::D);
            break;
        case(3):
            setArea(Solitaire::PileType::W);
            break;
        case(4):
            setArea(Solitaire::PileType::F);
            setPileIndex(0);
            break;
        case(5):
            setArea(Solitaire::PileType::F);
            setPileIndex(0);
            break;
        case(6):
            setArea(Solitaire::PileType::F);
            setPileIndex(0);
            break;
        }
    }
}

void Selection::moveDown() {
    switch(getArea()){
    case(Solitaire::PileType::D):
        setArea(Solitaire::PileType::T);
        setPileIndex(0);
        setCardIndex(0);
        break;
    case(Solitaire::PileType::W):
        setArea(Solitaire::PileType::T);
        setPileIndex(3);
        setCardIndex(0);
        break;
    case(Solitaire::PileType::F):
        switch(getPileIndex()){
        case(0):
            setArea(Solitaire::PileType::T);
            setPileIndex(4);
            setCardIndex(0);
            break;
        case(1):
            setArea(Solitaire::PileType::T);
            setPileIndex(5);
            setCardIndex(0);
            break;
        case(2):
            setArea(Solitaire::PileType::T);
            setPileIndex(6);
            setCardIndex(0);
            break;
        case(3):
            setArea(Solitaire::PileType::T);
            setPileIndex(6);
            setCardIndex(0);
            break;
        }
        break;
    case(Solitaire::PileType::T):
        setCardIndex(getCardIndex()+1);
        break;
    }
}

bool Selection::isSelected(Solitaire::PileType t, int pile, int card) const {
    return getArea() == t && getPileIndex() == pile && getCardIndex() == card;
}

Selection::sourceSnap srcSnap;
bool Selection::isSource(Solitaire::PileType t, int pile, int card) const {
    return srcSnap.type == t && srcSnap.pileI == pile && srcSnap.cardI;
}

void Selection::restoreSource(){
    if(!cardFrom) return;
    setArea(cardFrom->type);
    setPileIndex(cardFrom->pileI);
    setCardIndex(cardFrom->cardI);
}

bool Selection::pickCard(){
    if(holding) return false;
    cardFrom = sourceSnap{getArea(),getPileIndex(),getCardIndex()};
    holding = true;
    return true;
}

bool Selection::placeCard(){
    if(!holding) return false;
    holding = false;
    cardFrom.reset();
    return true;
}

bool Selection::cancel(){
    if(!holding) return false;
    restoreSource();
    holding=false;
    cardFrom.reset();
    return true;
}
