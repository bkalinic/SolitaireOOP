#include "../header/Deck.h"

Deck::Deck()
    :Pile(D){}

Deck::~Deck(){}

void CreateDeck(){}

void ShuffleDeck(){
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(this),std::end(this),rng);
}

size_t remainingCards() const{
    return deckVct.size();
}

/*cardPtr drawCard(*WASTEPILE DETEKTOR NEKAKAV*){
    if(!this.empty()){
        WASTEPILE.push_back(this.back());
        this.pop_back();
    }
}
*/
