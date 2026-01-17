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
    auto screen = ScreenInteractive::TerminalOutput();

    Deck deck;
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

    std::vector<Card> stock;
    std::vector<Card> waste;

    
    /*table[0] = { Card{Suit::P, Rank::As, true} };
    table[1] = { Card{Suit::H, Rank::Dva, false},
                        Card{Suit::K, Rank::Pet, true} };
    table[2] = { Card{Suit::T, Rank::Tri, true},
                        Card{Suit::P, Rank::Cetiri, true},
                        Card{Suit::H, Rank::Dama, true} };
    table[3] = { Card{Suit::K, Rank::Pet, true} };
    table[4] = { Card{Suit::T, Rank::Deset, true} };
    table[5] = { Card{Suit::P, Rank::Decko, true},
                        Card{Suit::H, Rank::Sedam, true} };
    table[6] = { Card{Suit::K, Rank::Devet, true} };*/

    waste = { Card{Suit::T, Rank::Osam, true} };

    auto renderer = Renderer([&] {
        auto stock_display = stock.empty() ? CreateEmptyPile() : CreateCardElement(stock.back());

        auto waste_display = waste.empty() ? CreateEmptyPile() : CreateCardElement(waste.back());

        std::vector<Element> foundation_elements(4);
        for (int i = 0; i < 4; i++) {
            foundation_elements.push_back(CreateFoundationPile(
                foundations[i].getVct().end(), Solitaire::Suit::H));
        }
        

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
                  hbox(std::move(foundation_elements))
                })
            });

        std::vector<Element> tableau_elements;
        for (int i = 0; i < 7; ++i) {
            Element pile_element;

            if (table[i].empty()) {
                pile_element = CreateEmptyPile();
            }
            else {
                std::vector<Element> cards_in_pile;
                for (const auto& card : table[i]) {
                    cards_in_pile.push_back(CreateCardElement(card));
                }
                pile_element = vbox(std::move(cards_in_pile));
            }

            auto labeled_pile = vbox({
              text("Pile " + std::to_string(i + 1)) | center,
              pile_element
                });

            tableau_elements.push_back(labeled_pile);

            if (i < 6) {
                tableau_elements.push_back(filler());
            }
        }

        auto tableau_section = vbox({
          text("Table") | center,
          hbox(std::move(tableau_elements))
            });

        return vbox({
          top_section,
          separator(),
          tableau_section,
          separator(),
          text("Solitaire - Use arrow keys, ESC to exit") | center | color(Color::GrayLight)
            }) | border | flex;
        });

    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
        });

    screen.Loop(component);

    return 0;
}