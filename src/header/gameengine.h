#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "deck.h"
#include "tablepile.h"
#include "foundationpile.h"
#include "stockpile.h"
#include "wastepile.h"
#include "command.h"
#include "types.h"
#include <vector>
#include <memory>
#include <stack>

class GameEngine {
private:
    unique_ptr<Deck> m_deck;

    vector<shared_ptr<TablePile>> m_tablePiles;
    vector<shared_ptr<FoundationPile>> m_foundationPiles;
    shared_ptr<StockPile> m_stockPile;
    shared_ptr<WastePile> m_wastePile;

    stack<shared_ptr<Command>> m_undoStack;
    stack<shared_ptr<Command>> m_redoStack;

    int m_score;
    int m_moveCount;
    bool m_gameWon;

public:
    GameEngine();

    void newGame();
    void restartGame();

    bool moveCard(CardPtr card, shared_ptr<Pile> source, shared_ptr<Pile> destination);
    bool moveCards(const vector<CardPtr>& cards, shared_ptr<Pile> source, shared_ptr<Pile> destination);
    
    bool canDrawFromStock() const;
    shared_ptr<Card> drawFromStock();
    void addCardToWaste(std::shared_ptr<Card> card);
    void resetStockFromWaste();

    void moveCardWithUndo(std::shared_ptr<Card> card,
        std::shared_ptr<Pile> from,
        std::shared_ptr<Pile> to);
    void moveCardWithUndo(const std::vector<std::shared_ptr<Card>>& cards,
        std::shared_ptr<Pile> from,
        std::shared_ptr<Pile> to);

    void undo();
    void redo();
    bool canUndo() const { return !m_undoStack.empty(); }
    bool canRedo() const { return !m_redoStack.empty(); }
    void clearUndoRedo();

    bool isGameWon() const { return m_gameWon; }
    int score() const { return m_score; }
    int moveCount() const { return m_moveCount; }

    int getStockSize() const;
    int getWasteSize() const;

    shared_ptr<TablePile> tablePile(int index);
    const vector<shared_ptr<TablePile>>& tablePiles() const { return m_tablePiles; }
    shared_ptr<FoundationPile> foundationPile(int index);
    const vector<shared_ptr<FoundationPile>>& foundationPiles() const { return m_foundationPiles; }
    shared_ptr<StockPile> stockPile() const { return m_stockPile; }
    shared_ptr<WastePile> wastePile() const { return m_wastePile; }

    bool isValidMove(CardPtr card, shared_ptr<Pile> source, shared_ptr<Pile> dest) const;
    bool isValidMoves(const vector<CardPtr>& cards, shared_ptr<Pile> source, shared_ptr<Pile> dest) const;
    bool isValidFoundationMove(shared_ptr<Card> card, shared_ptr<Pile> foundation);
    bool isValidTableMove(shared_ptr<Card> card, shared_ptr<Pile> table);

    void printGameState() const;

private:
    void initializePiles();
    void resetGameState();
    void updateScore(int points);
    void checkGameWon();
};

#endif