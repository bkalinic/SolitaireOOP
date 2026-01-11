#include "../header/gameengine.h"
#include <iostream>
#include <algorithm>
#include <QDebug>

GameEngine::GameEngine()
    : m_score(0), m_moveCount(0), m_gameWon(false) {

    m_deck = make_unique<Deck>();
    initializePiles();
}

void GameEngine::initializePiles() {
    for (int i = 0; i < 7; ++i) {
        m_tablePiles.push_back(make_shared<TablePile>());
    }
    for (int s = 0; s < 4; ++s) {
        Solitaire::Suit suit = static_cast<Solitaire::Suit>(s);
        m_foundationPiles.push_back(make_shared<FoundationPile>(suit));
    }
    m_stockPile = make_shared<StockPile>();
    m_wastePile = make_shared<WastePile>();
}

void GameEngine::newGame() {
    resetGameState();

    m_deck->createFullDeck();
    m_deck->shuffle();

    m_deck->dealToTable(m_tablePiles);

    while (!m_deck->isEmpty()) {
        m_stockPile->addCard(m_deck->drawCard());
    }

    m_moveCount = 0;
    m_score = 0;
    m_gameWon = false;

    std::cout << "Nova igra zapocinje!" << std::endl;
}

void GameEngine::restartGame() {
    newGame();
}

bool GameEngine::moveCard(CardPtr card, shared_ptr<Pile> source, shared_ptr<Pile> destination) {
    if (!card || !source || !destination) {
        return false;
    }
    if (!isValidMove(card, source, destination)) {
        return false;
    }
    
    vector<CardPtr> cards = { card };
    auto command = make_shared<MoveCommand>(cards, source, destination);
    command->execute();
    m_undoStack.push(command);
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }

    m_moveCount++;
    updateScore(10);

    auto tableSource = dynamic_pointer_cast<TablePile>(source);
    if (tableSource) {
        tableSource->flipTopCard();
    }

    checkGameWon();
    return true;
}

bool GameEngine::moveCards(const vector<CardPtr>& cards, shared_ptr<Pile> source, shared_ptr<Pile> destination) {
    if (cards.empty() || !source || !destination) {
        return false;
    }
    if (!isValidMoves(cards, source, destination)) {
        return false;
    }

    auto command = make_shared<MoveCommand>(cards, source, destination);

    command->execute();
    m_undoStack.push(command);
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }

    m_moveCount++;
    updateScore(10);

    auto tableSource = dynamic_pointer_cast<TablePile>(source);
    if (tableSource) {
        tableSource->flipTopCard();
    }

    checkGameWon();
    return true;
}

bool GameEngine::canDrawFromStock() const {
    return m_stockPile && !m_stockPile->isEmpty();
}

shared_ptr<Card> GameEngine::drawFromStock() {
    if (!m_stockPile || m_stockPile->cards().empty()) {
        return nullptr;
    }

    auto card = m_stockPile->removeTopCard();

    if (card) {
        card->setFaceUp(true);
    }

    return card;
}

void GameEngine::addCardToWaste(std::shared_ptr<Card> card) {
    if (!m_wastePile || !card) return;

    m_wastePile->addCard(card);
}

/*void GameEngine::drawSingleCard() {
    if (m_stockPile->isEmpty()) {
        resetStockFromWaste();
        return;
    }

    CardPtr card = m_stockPile->removeCard();
    if (card) {
        card->setFaceUp(true);
        m_wastePile->addCard(card);
    }
}

void GameEngine::drawThreeCards() {
    if (m_stockPile->isEmpty()) {
        resetStockFromWaste();
        return;
    }

    int cardsToDraw = min(3, static_cast<int>(m_stockPile->count()));
    for (int i = 0; i < cardsToDraw; ++i) {
        CardPtr card = m_stockPile->removeCard();
        if (card) {
            card->setFaceUp(true);
            m_wastePile->addCard(card);
        }
    }
}*/

void GameEngine::resetStockFromWaste() {
    if (!m_stockPile || !m_wastePile) return;

    auto wasteCards = m_wastePile->cards();

    for (auto it = wasteCards.rbegin(); it != wasteCards.rend(); ++it) {
        auto card = *it;
        card->setFaceUp(false);
        m_stockPile->addCard(card);
    }

    m_wastePile->clear();

    updateScore(-100);
}

void GameEngine::moveCardWithUndo(std::shared_ptr<Card> card,
    std::shared_ptr<Pile> from,
    std::shared_ptr<Pile> to) {
    if (!card || !from || !to) {
        std::cout << "moveCardWithUndo: invalid parameters" << std::endl;
        return;
    }
    std::vector<std::shared_ptr<Card>> cards = { card };
}

void GameEngine::moveCardWithUndo(const std::vector<std::shared_ptr<Card>>& cards,
    std::shared_ptr<Pile> from,
    std::shared_ptr<Pile> to) {
    if (cards.empty() || !from || !to) return;

    qDebug() << "=== GameEngine::moveCardWithUndo ===";
    qDebug() << "Pomicem" << cards.size() << "karata";

    if (cards.size() == 1) {
        qDebug() << "Karta:" << cards.front()->toString();
    }

    auto command = std::make_shared<MoveCommand>(cards, from, to);

    command->execute();

    m_undoStack.push(command);

    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }

    m_moveCount++;

    qDebug() << "Potez izvrsen. Ukupno poteza:" << m_moveCount;
}

void GameEngine::undo() {
    if (m_undoStack.empty()) {
        std::cout << "Nothing to undo" << std::endl;
        return;
    }

    auto command = m_undoStack.top();
    m_undoStack.pop();

    command->undo();

    m_redoStack.push(command);

    m_moveCount--;
    std::cout << "Undo performed. Undo stack: " << m_undoStack.size()
        << ", Redo stack: " << m_redoStack.size() << std::endl;

    updateScore(-10);
}

void GameEngine::redo() {
    if (m_redoStack.empty()) {
        std::cout << "Nothing to redo" << std::endl;
        return;
    }

    auto command = m_redoStack.top();
    m_redoStack.pop();

    command->execute();

    m_undoStack.push(command);

    m_moveCount++;
    std::cout << "Redo performed. Undo stack: " << m_undoStack.size()
        << ", Redo stack: " << m_redoStack.size() << std::endl;

    updateScore(10);
}

void GameEngine::clearUndoRedo() {
    while (!m_undoStack.empty()) m_undoStack.pop();
    while (!m_redoStack.empty()) m_redoStack.pop();
    std::cout << "Undo/redo cleared" << std::endl;
}

int GameEngine::getStockSize() const {
    return m_stockPile ? m_stockPile->count() : 0;
}

int GameEngine::getWasteSize() const {
    return m_wastePile ? m_wastePile->count() : 0;
}

shared_ptr<TablePile> GameEngine::tablePile(int index) {
    if (index >= 0 && index < m_tablePiles.size()) {
        return m_tablePiles[index];
    }
    return nullptr;
}

shared_ptr<FoundationPile> GameEngine::foundationPile(int index) {
    if (index >= 0 && index < m_foundationPiles.size()) {
        return m_foundationPiles[index];
    }
    return nullptr;
}

bool GameEngine::isValidMove(CardPtr card, shared_ptr<Pile> source, shared_ptr<Pile> dest) const {
    if (!card || !source || !dest) {
        return false;
    }
    bool cardFound = false;
    for (const auto& c : source->cards()) {
        if (c->id() == card->id()) {
            cardFound = true;
            break;
        }
    }
    if (!cardFound) {
        return false;
    }
    if (source->type() == Solitaire::PileType::Table) {
        auto table = dynamic_pointer_cast<TablePile>(source);
        if (!table->canRemoveCard()) {
            return false;
        }
    }
    else if (source->topCard()->id() != card->id()) {
        return false;
    }
    return dest->canAcceptCard(card);
}

bool GameEngine::isValidMoves(const vector<CardPtr>& cards,
    shared_ptr<Pile> source,
    shared_ptr<Pile> dest) const {
    if (cards.empty() || !source || !dest) {
        return false;
    }

    if (source->type() != Solitaire::PileType::Table) {
        return false;
    }

    auto tableSource = dynamic_pointer_cast<TablePile>(source);
    if (!tableSource) {
        return false;
    }

    for (const auto& card : cards) {
        bool found = false;
        for (const auto& c : source->cards()) {
            if (c->id() == card->id()) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return dest->canAcceptCards(cards);
}

bool GameEngine::isValidFoundationMove(shared_ptr<Card> card, shared_ptr<Pile> foundation) {
    if (!card || !foundation) return false;

    auto cards = foundation->cards();

    if (cards.empty()) {
        return card->value() == 1;
    }

    auto topCard = cards.back();

    bool topIsRed = (topCard->suit() == Solitaire::Suit::Herc || topCard->suit() == Solitaire::Suit::Karo);
    bool cardIsRed = (card->suit() == Solitaire::Suit::Herc || card->suit() == Solitaire::Suit::Karo);

    if (topIsRed == cardIsRed) {
        return false;
    }

    return card->value() == topCard->value() - 1;
}

bool isValidTableMove(shared_ptr<Card> card, shared_ptr<Pile> table) {
    if (!card || !table) return false;

    auto cards = table->cards();

    if (cards.empty()) {
        return card->value() == 13;
    }

    auto topCard = cards.back();

    bool topIsRed = (topCard->suit() == Solitaire::Suit::Herc || topCard->suit() == Solitaire::Suit::Karo);
    bool cardIsRed = (card->suit() == Solitaire::Suit::Herc || card->suit() == Solitaire::Suit::Karo);

    if (topIsRed == cardIsRed) {
        return false;
    }

    bool isOneLess = card->value() == topCard->value() - 1;
    return isOneLess;
}

void GameEngine::updateScore(int points) {
    m_score += points;
    if (m_score < 0) {
        m_score = 0;
    }
}

void GameEngine::checkGameWon() {
    for (const auto& foundation : m_foundationPiles) {
        if (!foundation->isComplete()) {
            m_gameWon = false;
            return;
        }
    }

    m_gameWon = true;
    updateScore(1000);
    cout << "Congratulations! You won the game!" << endl;
}

void GameEngine::resetGameState() {
    for (auto& pile : m_tablePiles) {
        pile->clear();
    }

    for (auto& pile : m_foundationPiles) {
        pile->clear();
    }

    m_stockPile->clear();
    m_wastePile->clear();
    m_deck->clear();

    m_undoStack = stack<shared_ptr<Command>>();
    m_redoStack = stack<shared_ptr<Command>>();
}

void GameEngine::printGameState() const {
    std::cout << "\n=== SOLITAIRE GAME STATE ===" << std::endl;
    std::cout << "Score: " << m_score << " | Moves: " << m_moveCount << std::endl;

    std::cout << "\nSTOCK: ";
    m_stockPile->print();

    std::cout << "WASTE: ";
    m_wastePile->print();

    std::cout << "\nFOUNDATIONS:" << std::endl;
    for (size_t i = 0; i < m_foundationPiles.size(); ++i) {
        std::cout << "  F" << (i + 1) << ": ";
        m_foundationPiles[i]->print();
    }

    std::cout << "\nTABLE PILES:" << std::endl;
    for (size_t i = 0; i < m_tablePiles.size(); ++i) {
        std::cout << "  T" << (i + 1) << ": ";
        m_tablePiles[i]->print();
    }

    if (m_gameWon) {
        std::cout << "GAME WON!" << std::endl;
    }
    std::cout << "============================" << std::endl;
}