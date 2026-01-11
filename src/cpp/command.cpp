#include "command.h"
#include <iostream>
#include <sstream>

MoveCommand::MoveCommand(std::shared_ptr<Card> card,
    std::shared_ptr<Pile> from,
    std::shared_ptr<Pile> to)
    : m_cards({card}), m_from(from), m_to(to) {
}

MoveCommand::MoveCommand(const std::vector<std::shared_ptr<Card>>& cards,
    std::shared_ptr<Pile> from,
    std::shared_ptr<Pile> to)
    : m_cards(cards), m_from(from), m_to(to) {
    for (const auto& card : m_cards) {
        if (card) {
            m_wasFaceUp.push_back(card->isFaceUp());
        }
        else {
            m_wasFaceUp.push_back(false);
        }
    }
}

void MoveCommand::execute() {
    if (m_cards.empty() || !m_from || !m_to) return;

    std::cout << "EXECUTE: " << getDescription() << std::endl;

    for (const auto& card : m_cards) {
        if (card && m_from->removeSpecificCard(card)) {
            m_to->addCard(card);
            card->setFaceUp(true);
        }
    }
}

void MoveCommand::undo() {
    if (m_cards.empty() || !m_from || !m_to) return;

    std::cout << "UNDO: " << getDescription() << std::endl;

    for (int i = static_cast<int>(m_cards.size()) - 1; i >= 0; i--) {
        const auto& card = m_cards[i];
        if (card && m_to->removeSpecificCard(card)) {
            m_from->addCard(card);
            card->setFaceUp(m_wasFaceUp[i]);
        }
    }
}

void MoveCommand::redo() {
    execute();
}

std::string MoveCommand::getDescription() const {
    std::stringstream ss;
    ss << "Move " << m_cards.size() << " card(s): ";

    for (size_t i = 0; i < m_cards.size(); i++) {
        if (i > 0) ss << ", ";
        ss << (m_cards[i] ? m_cards[i]->toString() : "unknown");
    }

    ss << " from " << (m_from ? m_from->toString() : "unknown")
        << " to " << (m_to ? m_to->toString() : "unknown");

    return ss.str();
}