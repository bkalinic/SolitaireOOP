#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <memory>
#include <QMap>
#include "../header/gameengine.h"
#include "carditem.h"
#include "pileitem.h"

class GameManager : public QObject {
    Q_OBJECT

public:
    static GameManager& instance();
    
    void setScene(QGraphicsScene* scene);
    QGraphicsScene* scene() const { return m_scene; }

    void initializeNewGame();
    bool attemptMove(CardItem* cardItem, PileItem* targetPile);
    void drawFromStock();
    void updateVisualsAfterDraw();
    void updateVisualsAfterStockReset();
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;

    GameEngine* engine() { return m_engine.get(); }
    const QList<PileItem*>& getPileItems() const { return m_pileItems; }
    const QList<CardItem*>& getCardItems() const { return m_cardItems; }

    CardItem* getCardItemForCard(std::shared_ptr<Card> card);
    PileItem* getPileItemForPile(std::shared_ptr<Pile> pile);
    PileItem* getPileItemForCard(CardItem* cardItem);

signals:
    void cardDrawn(CardPtr card);
    void stockEmpty();
    void wasteReset();
    void stockRefilled(int newSize);

    void wasteUpdated(int wasteSize);
    void stockUpdated(int stockSize);
    void gameStateChanged();
    void scoreUpdated(int score);
    void movesUpdated(int moves);
    void gameWon();

    void cardsDrawn(const std::vector<CardPtr>& cards);
    void drawFailed(const QString& reason);

    void undoAvailable(bool available);
    void redoAvailable(bool available);

    void cardSelected(CardPtr card);
    void cardDeselected();

private:
    GameManager(QObject* parent = nullptr);
    void createCardItems();
    void createPileItems();
    void connectCardSignals();
    void clearCardItems();
    void clearPileItems();

    vector<shared_ptr<Card>> getCardStack(CardItem* topCardItem, PileItem* pileItem);
    void moveCardStackVisual(CardItem* topCardItem, PileItem* sourcePile, PileItem* targetPile,
                            const std::vector<std::shared_ptr<Card>>& cards);
    void revealTopCardInPile(PileItem* pileItem);

    QGraphicsScene* m_scene = nullptr;
    std::unique_ptr<GameEngine> m_engine;
    QList<CardItem*> m_cardItems;
    QList<PileItem*> m_pileItems;
    QMap<std::shared_ptr<Card>, CardItem*> m_cardMap;
    QMap<std::shared_ptr<Pile>, PileItem*> m_pileMap;

    CardItem* m_selectedCard;

    void onCardClicked(CardItem* cardItem);
    void onCardDropped(CardItem* cardItem);
    void onCardDragged(CardItem* cardItem, const QPointF& offset);

    PileItem* findPileAtPosition(const QPointF& position);
    bool canCardBeDroppedOnPile(CardItem* cardItem, PileItem* pileItem);
    void resetCardPosition(CardItem* cardItem);
};

#endif