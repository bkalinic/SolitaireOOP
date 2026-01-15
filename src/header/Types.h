#ifndef TYPES_H
#define TYPES_H
#include <string>

namespace Solitaire{
    enum class Suit{H,K,P,T};
    enum class Rank{1,2,3,4,5,6,7,8,9,10,11,12,13};
    enum class PileType{T,F,W,S};
    inline std::string stringSuit(Suit s){
        switch(s){
        case(Suit::H):return "Herc";
        case(Suit::K):return "Karo";
        case(Suit::P):return "Pik";
        case(Suit::T):return "Tref";
        }
    }
    inline std::string stringRank(Rank r){
        switch(r){
        case(Rank::1):return "As";
        case(Rank::11):return "Decko";
        case(Rank::12):return "Dama";
        case(Rank::13):return "Kralj";
        default: return std::to_string(static_cast<int>(r));
        }
    }
}

#endif // TYPES_H


