#ifndef TYPES_H
#define TYPES_H

#include <string>

namespace Solitaire {
    enum class Suit { Herc, Karo, Tref, Pik };
    enum class Rank {
        As = 1, Dvojka, Trica, Cetvorka, Petica, Sestica, Sedmica,
        Osmica, Devetka, Desetka, Jack, Dama, Kralj
    };
    enum class PileType { Table, Foundation, Stock, Waste };

    inline std::string suitToString(Suit suit) {
        switch (suit) {
        case Suit::Herc: return "Herc";
        case Suit::Karo: return "Karo";
        case Suit::Tref: return "Tref";
        case Suit::Pik:  return "Pik";
        default: return "Unknown";
        }
    }
    inline std::string rankToString(Rank rank) {
        switch (rank) {
        case Rank::As:    return "As";
        case Rank::Jack:  return "Jack";
        case Rank::Dama:  return "Dama";
        case Rank::Kralj: return "Kralj";
        default: return std::to_string(static_cast<int>(rank));
        }
    }
}

#endif