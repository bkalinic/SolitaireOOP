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
#include "src/header/TablePile.h"
#include "src/header/Types.h"
#include "src/header/WastePile.h"
#include "src/header/Selection.h"
#include <vector>
#include <string>

using namespace ftxui;
using namespace Solitaire;

Element CreateCardElement(const cardPtr card) {
    if (!(*card).isFaceUp()) {
        return vbox({
          text("┌────┐"),
          text("│░░░░│") | color(Color::BlueLight),
          text("│░░░░│") | color(Color::BlueLight),
          text("└────┘")
            }) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
    }

    std::string value = stringRank((*card).getRank());
    if (value == "10") {
        value = "10";
    }
    else {
        value = value + " ";
    }

    return vbox({
      text("┌────┐"),
      hbox({text("│"), text(value + (*card).getSymbol()) | color((*card).getColor()), text(" │")}),
      text("│    │"),
      text("└────┘")
        }) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
}

Element CreateEmptyPile() {
    return vbox({
      text("╔════╗"),
      text("║    ║"),
      text("║    ║"),
      text("╚════╝")
        }) | color(Color::GrayLight) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
}

Element CreateEmptyFoundation(Solitaire::Suit suit) {
    return vbox({
      text("┌────┐"),
      text("║    ║"),
      text("║    ║"),
      text("╚════╝")
        }) | color(Color::GrayLight) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
}

Element CreateFoundationPile(const cardPtr top_card, Solitaire::Suit suit) {
    std::string foundation_symbol;
    switch (suit) {
    case Solitaire::Suit::H: foundation_symbol = "H"; break;
    case Solitaire::Suit::K: foundation_symbol = "K"; break;
    case Solitaire::Suit::T: foundation_symbol = "T"; break;
    case Solitaire::Suit::P: foundation_symbol = "P"; break;
    }

    if (stringRank((*top_card).getRank()) == "") {
        return vbox({
          text("┌────┐"),
          text("│ " + foundation_symbol + "  │") | color(Color::White),
          text("│    │"),
          text("└────┘")
            }) | border | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5);
    }

    return CreateCardElement(top_card) | border;
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
            ? CreateEmptyPile() : CreateCardElement((deck.getDeckVct()).back());

        auto waste_display = (waste.getWasteVct()).empty()
            ? CreateEmptyPile() : CreateCardElement((waste.getWasteVct()).back());

        std::vector<Element> foundation_elements = {
        foundations[0].getVct().empty() ? CreateEmptyPile() :
            CreateFoundationPile(foundations[0].getVct().back(), Solitaire::Suit::H),
        foundations[1].getVct().empty() ? CreateEmptyPile() :
            CreateFoundationPile(foundations[1].getVct().back(), Solitaire::Suit::K),
        foundations[2].getVct().empty() ? CreateEmptyPile() :
            CreateFoundationPile(foundations[2].getVct().back(), Solitaire::Suit::T),
        foundations[3].getVct().empty() ? CreateEmptyPile() :
            CreateFoundationPile(foundations[3].getVct().back(), Solitaire::Suit::P),
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
                pile_element = CreateEmptyPile();
            }
            else {
                std::vector<Element> cards_in_pile;
                for (const auto& card : table[i].getPileVct()) {
                    cards_in_pile.push_back(CreateCardElement(card));
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

    Selection selector;

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::ArrowLeft) {Selection::moveLeft(); return true;}
        if (event == Event::ArrowRight) {Selection::moveRight(); return true;}
        if (event == Event::ArrowUp) {Selection::moveUp(); return true;}
        if (event == Event::ArrowDown) {Selection::moveDown(); return true;}
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });

    screen.Loop(component);

    return 0;
}
