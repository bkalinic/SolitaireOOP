// SolitaireFixed.cpp : Defines the entry point for the application.
//

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "SolitaireFixed.h"
#include "src/header/Card.h"
#include "src/header/Deck.h"
#include "src/header/FoundationPile.h"
#include "src/header/GameEngine.h"
#include "src/header/TablePile.h"
#include "src/header/Types.h"
#include "src/header/WastePile.h"
#include "src/header/Selection.h"
#include <vector>
#include <string>

using namespace ftxui;
using namespace Solitaire;

Selection selector;

Element CreateCardElement(const cardPtr card, Solitaire::PileType type, int pileIter, int cardInPile) {
    bool selected = selector.isSelected(type, pileIter, cardInPile);
    bool source = selector.isSource(type,pileIter,cardInPile);
    auto cardFdown = vbox({
          text("┌────┐"),
          text("│░░░░│") | color(Color::BlueLight),
          text("│░░░░│") | color(Color::BlueLight),
          text("└────┘")
            }) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
    if (!(*card).isFaceUp()) {
        if(selected){
        cardFdown = cardFdown | color(Color::Yellow);
        }
        if(source){
        cardFdown = cardFdown | color(Color::Green);
        }
        return cardFdown;
    }

    std::string value = stringRank((*card).getRank());
    if (value == "10") {
        value = "10";
    }
    else {
        value = value + " ";
    }
    auto cardFup = vbox({
      text("┌────┐"),
      hbox({text("│"), text(value + (*card).getSymbol()) | color((*card).getColor()), text(" │")}),
      text("│    │"),
      text("└────┘")
        }) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);

    if(selected){
        cardFup = cardFup | color(Color::Yellow);
    }
    if(source){
        cardFup = cardFup | color(Color::Green);
    }

    return cardFup;

}

Element CreateEmptyPile(Solitaire::PileType type,int pileIter, int cardInPile) {
    bool selected = selector.isSelected(type, pileIter, cardInPile);
    bool source = selector.isSource(type,pileIter,cardInPile);
    auto emptyPileCard = vbox({
      text("╔════╗"),
      text("║    ║"),
      text("║    ║"),
      text("╚════╝")
        }) | color(Color::GrayLight) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
    if(selected){
        emptyPileCard = emptyPileCard | bgcolor(Color::Yellow);
    }
    if(source){
        emptyPileCard = emptyPileCard | bgcolor(Color::Green);
    }
    return  emptyPileCard;
}

Element CreateFoundationPile(const cardPtr top_card, Solitaire::Suit suit, Solitaire::PileType type, int pileIter) {
    std::string foundation_symbol;
    switch (suit) {
    case Solitaire::Suit::H: foundation_symbol = "H"; break;
    case Solitaire::Suit::K: foundation_symbol = "K"; break;
    case Solitaire::Suit::T: foundation_symbol = "T"; break;
    case Solitaire::Suit::P: foundation_symbol = "P"; break;
    }

    bool selected = selector.isSelected(type, pileIter);
    bool source = selector.isSource(type, pileIter);

    if (stringRank((*top_card).getRank()) == "") {
        auto fndSlot = vbox({
          text("┌────┐"),
          text("│ " + foundation_symbol + "  │") | color(Color::White),
          text("│    │"),
          text("└────┘")
            }) | border | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
        if(selected){
            fndSlot = fndSlot | bgcolor(Color::Yellow);
        }
        return fndSlot;
    }


    return CreateCardElement(top_card, type) | border;
}

int main() {
    auto screen = ScreenInteractive::Fullscreen();

    Deck deck;
    deck.shuffleDeck();

    std::vector<TablePile> table;
    for (int i = 0; i < 7; i++) {
        TablePile tmp = TablePile(deck, i+1);
        table.push_back(tmp);
    }

    std::vector<FoundationPile> foundations;
    foundations.push_back(FoundationPile(Solitaire::Suit::H));
    foundations.push_back(FoundationPile(Solitaire::Suit::K));
    foundations.push_back(FoundationPile(Solitaire::Suit::T));
    foundations.push_back(FoundationPile(Solitaire::Suit::P));

    WastePile waste;

    auto renderer = Renderer([&] {
        auto stock_display = (deck.getDeckVct()).empty()
            ? CreateEmptyPile(Solitaire::PileType::D) : CreateCardElement((deck.getDeckVct()).back(),Solitaire::PileType::D);

        auto waste_display = (waste.getWasteVct()).empty()
            ? CreateEmptyPile(Solitaire::PileType::W) : CreateCardElement((waste.getWasteVct()).back(),Solitaire::PileType::W);

        std::vector<Element> foundation_elements = {
        foundations[0].getVct().empty() ? CreateEmptyPile(Solitaire::PileType::F) :
            CreateFoundationPile(foundations[0].getVct().back(), Solitaire::Suit::H, Solitaire::PileType::F, 0),
        foundations[1].getVct().empty() ? CreateEmptyPile(Solitaire::PileType::F) :
            CreateFoundationPile(foundations[1].getVct().back(), Solitaire::Suit::K,  Solitaire::PileType::F, 1),
        foundations[2].getVct().empty() ? CreateEmptyPile(Solitaire::PileType::F) :
            CreateFoundationPile(foundations[2].getVct().back(), Solitaire::Suit::T,  Solitaire::PileType::F, 2),
        foundations[3].getVct().empty() ? CreateEmptyPile(Solitaire::PileType::F) :
            CreateFoundationPile(foundations[3].getVct().back(), Solitaire::Suit::P,  Solitaire::PileType::F, 3),
        };

        auto top_section = hbox({
            vbox({
                text("Stock"),
                stock_display
            }),
            filler(),
            vbox({
                text("Waste"),
                waste_display
            }),
            filler(),
            vbox({
                text("Foundations"),
                hbox(foundation_elements)
            })
        });

        std::vector<Element> table_elements;
        for (int i = 0; i < 7; ++i) {
            Element pile_element;

            if ((table[i].getPileVct()).empty()) {
                pile_element = CreateEmptyPile(Solitaire::PileType::T,i);
            }
            else {
                std::vector<Element> cards_in_pile;
                int tablePLen=table[i].getPileSize();
                for (const auto& card : table[i].getPileVct()) {
                    cards_in_pile.push_back(CreateCardElement(card,Solitaire::PileType::T,i,tablePLen));
                }
                pile_element = vbox(cards_in_pile);
            }

            auto labeled_pile = vbox({
              text("Pile " + std::to_string(i + 1)) | center,
              pile_element
                });

            table_elements.push_back(labeled_pile);

            if (i < 6) {
                table_elements.push_back(filler());
            }
        }

        auto table_section = vbox({
            text("Table") | center,
            hbox(table_elements)
        });

        if (screen.dimx() < 100 || screen.dimy() < 48) {
            return vbox({
                filler(),
                text("Prozor je premalen!") | bold | center,
                text("Minimalna velicina: 100x30") | center,
                filler(),
            }) | border;
        }

        return vbox({
            top_section,
            separator(),
            table_section,
            separator(),
            text("Solitaire - Pritisni ESC za izlaz") | center | color(Color::GrayLight)
        }) | border | flex;
    });

    GameEngine gameengine(deck, waste, table, foundations);
    for (int i = 0; i < 7; i++) {
        gameengine.gameStart(i);
    }

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::ArrowLeft) {selector.moveLeft(); return true;}
        if (event == Event::ArrowRight) {selector.moveRight(); return true;}
        if (event == Event::ArrowUp) {selector.moveUp(); return true;}
        if (event == Event::ArrowDown) {selector.moveDown(); return true;}
        if (event == Event::Return) {gameengine.handleEnter(selector); return true;}
        if (event == Event::Escape) {
            if(selector.getHold()){
                gameengine.cancel(selector);
                return true;
            }else{
                screen.Exit();
                return true;
            }
        }
        return false;
    });

    screen.Loop(component);

    return 0;
}
