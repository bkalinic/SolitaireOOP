#ifndef TYPES_H
#define TYPES_H
#include <string>

namespace Solitaire {
    enum class Suit { H, K, P, T };
    enum class Rank { As, Dva, Tri, Cetiri, Pet, Sest, Sedam, Osam, Devet, Deset, Decko, Dama, Kralj };
    enum class PileType { T, F, W, D };
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
        case(Rank::As):return "As";
        case(Rank::Decko):return "Decko";
        case(Rank::Dama):return "Dama";
        case(Rank::Kralj):return "Kralj";
        default: return std::to_string(static_cast<int>(r)+1);
        }
    }
}

#endif // TYPES_H


