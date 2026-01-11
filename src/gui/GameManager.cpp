#include "../guiHeader/GameManager.h"
#include <QGraphicsScene>

GameManager& GameManager::instance() {
    static GameManager instance;
    return instance;
}

GameManager::GameManager(QObject* parent)
    : QObject(parent), m_engine(std::make_unique<GameEngine>()) {
}

void GameManager::setScene(QGraphicsScene* scene) {
    m_scene = scene;
    if (m_scene) {
        qDebug() << "GameManager: scene postavljena";
    }
    else {
        qWarning() << "GameManager: scene je null!";
    }
}

void GameManager::initializeNewGame() {
    //m_engine->newGame();
    qDebug() << "=== initializeNewGame ===";

    clearCardItems();
    clearPileItems();

    if (m_engine) {
        m_engine->clearUndoRedo();
        m_engine->newGame();
    }

    createPileItems();
    createCardItems();
    connectCardSignals();

    emit gameStateChanged();
    emit scoreUpdated(0);
    emit movesUpdated(0);

    qDebug() << "Nova igra inicijalizirana";
}

void GameManager::onCardClicked(CardItem* cardItem) {
    if (!cardItem) return;

    qDebug() << "=== onCardClicked ===";
    qDebug() << "Karta:" << cardItem->getCard()->toString();
    qDebug() << "Face up:" << cardItem->getCard()->isFaceUp();

    PileItem* sourcePile = getPileItemForCard(cardItem);
    if (sourcePile && sourcePile->getPileType() == PileItem::Stock) {
        qDebug() << "Kliknut stock pile - vucem kartu...";
        drawFromStock();

        if (m_selectedCard) {
            m_selectedCard->setHighlighted(false);
        }
        m_selectedCard = nullptr;

        emit cardDeselected();
        return;
    }

    if (!cardItem->getCard()->isFaceUp()) {
        qDebug() << "Karta je okrenuta nadolje i nije u stocku, ignoriram";
        return;
    }

    m_selectedCard = cardItem;
    cardItem->setHighlighted(true);

    qDebug() << "Karta selektirana, pile:"
        << (sourcePile ? sourcePile->getPileType() : NULL);

    emit cardSelected(cardItem->getCard());
}

void GameManager::onCardDropped(CardItem* cardItem) {
    if (!cardItem || !m_selectedCard || m_selectedCard != cardItem) {
        return;
    }

    qDebug() << "=== onCardDropped ===";
    qDebug() << "Karta:" << cardItem->getCard()->toString();

    static QPointF lastMousePos;

    QRectF cardBounds = cardItem->sceneBoundingRect();
    QPointF dropPos = cardBounds.center();

    qDebug() << "Drop position (card center):" << dropPos;
    qDebug() << "Card bounds:" << cardBounds;

    PileItem* targetPile = findPileAtPosition(dropPos);

    if (!targetPile) {
        qDebug() << "Nije pronadjen pile po centru, trazim po cijeloj karti...";

        for (PileItem* pileItem : m_pileItems) {
            if (!pileItem) continue;

            QRectF pileRect = pileItem->sceneBoundingRect();
            if (pileRect.intersects(cardBounds)) {
                targetPile = pileItem;
                qDebug() << "Pronadjen pile po preklapanju:" << targetPile->getPileType();
                break;
            }
        }
    }

    if (targetPile) {
        qDebug() << "Ciljni pile:" << targetPile->getPileType();

        PileItem* sourcePile = getPileItemForCard(cardItem);
        qDebug() << "Izvorni pile:" << (sourcePile ? sourcePile->getPileType() : NULL);

        if (sourcePile == targetPile) {
            qDebug() << "Isti pile, vracam na mjesto";
            resetCardPosition(cardItem);

            if (m_selectedCard) {
                m_selectedCard->setHighlighted(false);
                m_selectedCard = nullptr;
            }
            emit cardDeselected();
            return;
        }

        if (attemptMove(cardItem, targetPile)) {
            qDebug() << "Potez uspjesan!";
        }
        else {
            qDebug() << "Potez neuspjesan, vracam kartu...";
            resetCardPosition(cardItem);
        }
    }
    else {
        qDebug() << "Nije pronadjen ciljni pile, vracam kartu...";
        resetCardPosition(cardItem);
    }

    if (m_selectedCard) {
        m_selectedCard->setHighlighted(false);
        m_selectedCard = nullptr;
    }

    emit cardDeselected();
}

void GameManager::onCardDragged(CardItem* cardItem, const QPointF& offset) {
    if (!cardItem) return;

    auto* sourcePileItem = getPileItemForCard(cardItem);
    if (!sourcePileItem || sourcePileItem->getPileType() != PileItem::Table) {
        return;
    }

    bool isTopCard = sourcePileItem->isTopCard(cardItem);
    if (!isTopCard) {
        cardItem->setPos(cardItem->pos() - offset);
        return;
    }

    for (PileItem* pileItem : m_pileItems) {
        if (pileItem && pileItem != sourcePileItem) {
            bool canAccept = canCardBeDroppedOnPile(cardItem, pileItem);
            pileItem->setHighlight(canAccept);
        }
    }
}

bool GameManager::canCardBeDroppedOnPile(CardItem* cardItem, PileItem* pileItem) {
    if (!cardItem || !pileItem) return false;

    auto card = cardItem->getCard();
    auto sourcePile = getPileItemForCard(cardItem)->getPile();
    auto destPile = pileItem->getPile();

    return m_engine->isValidMove(card, sourcePile, destPile);
}

void GameManager::resetCardPosition(CardItem* cardItem) {
    if (!cardItem) return;

    PileItem* originalPile = getPileItemForCard(cardItem);
    if (originalPile) {
        originalPile->updateLayout();
    }
}

void GameManager::createPileItems() {
    auto stockItem = new PileItem(m_engine->stockPile(), PileItem::Stock);
    m_scene->addItem(stockItem);
    m_pileItems.append(stockItem);
    m_pileMap[m_engine->stockPile()] = stockItem;
    qDebug() << "StockPileItem dodan na scenu";

    auto wasteItem = new PileItem(m_engine->wastePile(), PileItem::Waste);
    m_scene->addItem(wasteItem);
    m_pileItems.append(wasteItem);
    m_pileMap[m_engine->wastePile()] = wasteItem;
    qDebug() << "WastePileItem dodan na scenu";

    for (int i = 0; i < 4; ++i) {
        auto foundation = m_engine->foundationPile(i);
        auto item = new PileItem(foundation, PileItem::Foundation);
        m_scene->addItem(item);
        m_pileItems.append(item);
        m_pileMap[foundation] = item;
        qDebug() << "FoundationPileItem" << i << "dodan na scenu";
    }

    for (int i = 0; i < 7; ++i) {
        auto table = m_engine->tablePile(i);
        auto item = new PileItem(table, PileItem::Table);
        m_scene->addItem(item);
        m_pileItems.append(item);
        m_pileMap[table] = item;
        qDebug() << "TablePileItem" << i << "dodan na scenu";
    }
    qDebug() << "Ukupno pile items dodano na scenu:" << m_pileItems.size();
}

void GameManager::createCardItems() {
    qDebug() << "=== GameManager::createCardItems ===";

    if (!m_scene) {
        qCritical() << "m_scene je null!";
        return;
    }

    for (auto* cardItem : m_cardItems) {
        if (cardItem->scene()) {
            cardItem->scene()->removeItem(cardItem);
        }
        delete cardItem;
    }
    m_cardItems.clear();
    m_cardMap.clear();

    auto stockPile = m_engine->stockPile();
    if (stockPile) {
        auto cards = stockPile->cards();
        qDebug() << "Stock pile ima" << cards.size() << "karata";

        PileItem* pileItem = nullptr;
        for (auto* item : m_pileItems) {
            if (item->getPileType() == PileItem::Stock) {
                pileItem = item;
                break;
            }
        }

        for (const auto& card : cards) {
            auto* cardItem = new CardItem(card, pileItem);

            if (card) {
                card->setFaceUp(false);
                cardItem->updateAppearance();
            }

            if (pileItem) {
                cardItem->setPos(0, 0);
                cardItem->setZValue(0);
            }
            else {
                m_scene->addItem(cardItem);
                cardItem->setPos(50, 50);
            }

            m_cardItems.append(cardItem);
            m_cardMap[card] = cardItem;

            qDebug() << "Stock karta:" << (card ? card->toString() : "null")
                << "na" << cardItem->pos();
        }
    }

    int tableStartX = 200;
    int tableStartY = 100;

    for (int i = 0; i < 7; ++i) {
        auto tablePile = m_engine->tablePile(i);
        if (!tablePile) continue;

        auto cards = tablePile->cards();
        qDebug() << "Table" << i << "ima" << cards.size() << "karata";

        PileItem* pileItem = nullptr;
        for (auto* item : m_pileItems) {
            if (item->getPileType() == PileItem::Table &&
                dynamic_cast<TablePile*>(item->getPile().get()) == tablePile.get()) {
                pileItem = item;
                break;
            }
        }

        for (size_t j = 0; j < cards.size(); j++) {
            const auto& card = cards[j];
            auto* cardItem = new CardItem(card, pileItem);

            bool isFaceUp = (j == cards.size() - 1);
            if (card) {
                card->setFaceUp(isFaceUp);
                cardItem->updateAppearance();
            }

            if (pileItem) {
                int yOffset = j * 25;
                cardItem->setPos(0, yOffset);
                cardItem->setZValue(j);

                cardItem->setFlag(QGraphicsItem::ItemIsMovable, isFaceUp);
            }
            else {
                m_scene->addItem(cardItem);
                int x = 200 + (i * 120);
                int y = 100 + (j * 25);
                cardItem->setPos(x, y);
            }

            m_cardItems.append(cardItem);
            m_cardMap[card] = cardItem;
        }
    }

    int foundationStartX = 200;
    int foundationStartY = 400;

    for (int i = 0; i < 4; ++i) {
        auto foundationPile = m_engine->foundationPile(i);
        if (!foundationPile) continue;

        auto cards = foundationPile->cards();
        qDebug() << "Foundation" << i << "ima" << cards.size() << "karata";

        PileItem* pileItem = nullptr;
        for (auto* item : m_pileItems) {
            if (item->getPileType() == PileItem::Foundation &&
                dynamic_cast<FoundationPile*>(item->getPile().get()) == foundationPile.get()) {
                pileItem = item;
                break;
            }
        }

        for (size_t j = 0; j < cards.size(); j++) {
            const auto& card = cards[j];
            if (!card) continue;

            auto* cardItem = new CardItem(card, pileItem);

            card->setFaceUp(true);
            cardItem->updateAppearance();

            if (pileItem) {
                cardItem->setPos(0, 0);
                cardItem->setZValue(j);

                cardItem->setFlag(QGraphicsItem::ItemIsMovable, false);
            }
            else {
                m_scene->addItem(cardItem);
                int x = 500 + (i * 120);
                int y = 50;
                cardItem->setPos(x, y);
            }

            m_cardItems.append(cardItem);
            m_cardMap[card] = cardItem;
        }
    }

    auto wastePile = m_engine->wastePile();
    if (wastePile) {
        auto cards = wastePile->cards();
        qDebug() << "Waste pile ima" << cards.size() << "karata";

        PileItem* pileItem = nullptr;
        for (auto* item : m_pileItems) {
            if (item->getPileType() == PileItem::Waste) {
                pileItem = item;
                break;
            }
        }

        if (!cards.empty()) {
            const auto& topCard = cards.back();
            if (topCard) {
                auto* cardItem = new CardItem(topCard, pileItem);

                topCard->setFaceUp(true);
                cardItem->updateAppearance();

                if (pileItem) {
                    cardItem->setPos(0, 0);
                    cardItem->setZValue(100);

                    cardItem->setFlag(QGraphicsItem::ItemIsMovable, true);
                }
                else {
                    m_scene->addItem(cardItem);
                    cardItem->setPos(180, 50);
                }

                m_cardItems.append(cardItem);
                m_cardMap[topCard] = cardItem;
            }

            for (size_t i = 0; i < cards.size() - 1; i++) {
                const auto& card = cards[i];
                if (!card) continue;

                auto* cardItem = new CardItem(card, pileItem);

                card->setFaceUp(false);
                cardItem->updateAppearance();

                if (pileItem) {
                    cardItem->setPos(0, 0);
                    cardItem->setZValue(-i - 1);
                    cardItem->setVisible(false);
                }
                else {
                    m_scene->addItem(cardItem);
                    cardItem->setPos(180, 50);
                    cardItem->setZValue(-1);
                }

                m_cardItems.append(cardItem);
                m_cardMap[card] = cardItem;
            }
        }
    }

    qDebug() << "Ukupno CardItem-a:" << m_cardItems.size();

    qDebug() << "Ukupno kreirano CardItem-a:" << m_cardItems.size();
    qDebug() << "Scene items:" << m_scene->items().size();

    auto sceneItems = m_scene->items();
    int visibleCards = 0;
    int hiddenCards = 0;
    for (auto* item : sceneItems) {
        if (auto* cardItem = dynamic_cast<CardItem*>(item)) {
            if (cardItem->isCardFaceUp()) {
                visibleCards++;
            }
            else {
                hiddenCards++;
            }
        }
    }
    qDebug() << "Vidljivih karata:" << visibleCards;
    qDebug() << "Skrivenih karata:" << hiddenCards;
}

void GameManager::connectCardSignals() {
    qDebug() << "Povezujem signale za karte...";

    for (CardItem* cardItem : m_cardItems) {
        if (!cardItem) continue;

        connect(cardItem, &CardItem::cardClicked,
            this, &GameManager::onCardClicked);

        connect(cardItem, &CardItem::cardDropped,
            this, &GameManager::onCardDropped);

        connect(cardItem, &CardItem::cardDragged,
            this, &GameManager::onCardDragged);
    }

    qDebug() << "Signali povezani za" << m_cardItems.size() << "karata";
}

void GameManager::clearCardItems() {
    qDebug() << "=== clearCardItems ===";
    qDebug() << "Brisem" << m_cardItems.size() << "CardItem objekata";

    int removedFromScene = 0;
    int deletedCount = 0;

    for (auto* cardItem : m_cardItems) {
        if (!cardItem) continue;

        if (cardItem->scene()) {
            cardItem->scene()->removeItem(cardItem);
            removedFromScene++;
        }

        delete cardItem;
        deletedCount++;
    }

    m_cardItems.clear();
    m_cardMap.clear();

    qDebug() << "Uklonjeno sa scene:" << removedFromScene;
    qDebug() << "Obrisano:" << deletedCount;
    qDebug() << "CardItem-i ocisceni";
}

void GameManager::clearPileItems() {
    qDebug() << "=== clearPileItems ===";

    int removedCount = 0;

    for (auto* pileItem : m_pileItems) {
        if (!pileItem) continue;

        if (pileItem->scene()) {
            pileItem->scene()->removeItem(pileItem);
        }

        delete pileItem;
        removedCount++;
    }

    m_pileItems.clear();
    m_pileMap.clear();

    qDebug() << "Obrisano" << removedCount << "PileItem objekata";
}

PileItem* GameManager::findPileAtPosition(const QPointF& position) {
    qDebug() << "Trazim pile na:" << position;

    for (PileItem* pileItem : m_pileItems) {
        if (!pileItem) continue;

        QRectF pileBounds = pileItem->sceneBoundingRect();
        QRectF expandedBounds = pileBounds.adjusted(-20, -20, 20, 20);

        if (expandedBounds.contains(position)) {
            qDebug() << "Pronadjen pile:" << pileItem->getPileType();
            return pileItem;
        }
    }
    for (CardItem* cardItem : m_cardItems) {
        if (cardItem && cardItem->sceneBoundingRect().contains(position)) {
            return getPileItemForCard(cardItem);
        }
    }
    return nullptr;
}

bool GameManager::attemptMove(CardItem* cardItem, PileItem* targetPile) {
    if (!cardItem || !targetPile) return false;

    auto card = cardItem->getCard();
    auto sourcePile = getPileItemForCard(cardItem)->getPile();
    auto destPile = targetPile->getPile();
    PileItem* sourcePileItem = getPileItemForCard(cardItem);

    qDebug() << "=== attemptMove ===";
    qDebug() << "Karta:" << card->toString();
    qDebug() << "Izvor:" << sourcePileItem->getPileType();
    qDebug() << "Cilj:" << targetPile->getPileType();

    if (targetPile->getPileType() != PileItem::Table &&
        targetPile->getPileType() != PileItem::Foundation) {
        qDebug() << "Ciljni pile ne prihvaca stackove!";
        return false;
    }

    bool isValid = m_engine->isValidMove(card, sourcePile, destPile);
    qDebug() << "Engine kaze validno:" << isValid;

    if (isValid) {
        vector<shared_ptr<Card>> cardsToMove = getCardStack(cardItem, sourcePileItem);

        qDebug() << "Pomicem stack od" << cardsToMove.size() << "karata";

        m_engine->moveCardWithUndo(cardsToMove, sourcePile, destPile);

        moveCardStackVisual(cardItem, sourcePileItem, targetPile, cardsToMove);

        if (sourcePileItem->getPileType() == PileItem::Table) {
            revealTopCardInPile(sourcePileItem);
        }

        emit gameStateChanged();
        return true;
    }

    qDebug() << "Potez nije validan!";
    return false;
}

vector<shared_ptr<Card>> GameManager::getCardStack(CardItem* topCardItem, PileItem* pileItem) {
    std::vector<std::shared_ptr<Card>> stack;

    auto pileCards = pileItem->getPile()->cards();
    auto topCard = topCardItem->getCard();

    bool found = false;
    for (const auto& card : pileCards) {
        if (card == topCard) found = true;
        if (found) stack.push_back(card);
    }
    return stack;
}

void GameManager::moveCardStackVisual(CardItem* topCardItem, PileItem* sourcePile, PileItem* targetPile,
    const std::vector<std::shared_ptr<Card>>& cards) {
    for (const auto& card : cards) {
        if (auto* cardItem = m_cardMap.value(card)) {
            sourcePile->removeCardItem(cardItem);
            targetPile->addCardItem(cardItem);
        }
    }
    sourcePile->updateLayout();
    targetPile->updateLayout();
}

void GameManager::revealTopCardInPile(PileItem* pileItem) {
    auto pile = pileItem->getPile();
    if (!pile || pile->cards().empty()) return;

    auto topCard = pile->cards().back();
    if (auto* cardItem = m_cardMap.value(topCard)) {
        topCard->setFaceUp(true);
        cardItem->updateAppearance();

        qDebug() << "Otkrivena karta:" << topCard->toString();
    }
}

void GameManager::drawFromStock() {
    if (!m_engine) {
        emit drawFailed("GameEngine nije inicijaliziran");
        return;
    }

    qDebug() << "=== drawFromStock ===";

    int stockBefore = m_engine->getStockSize();
    int wasteBefore = m_engine->getWasteSize();

    if (m_engine->getStockSize() == 0) {
        if (m_engine->getWasteSize() > 0) {
            qDebug() << "Stock prazan, resetiram iz wastea...";
            m_engine->resetStockFromWaste();
            updateVisualsAfterStockReset();

            emit stockUpdated(m_engine->getStockSize());
            emit wasteUpdated(0);
            emit gameStateChanged();
        }
        else {
            qDebug() << "Stock i waste su prazni!";
        }
        return;
    }

    auto drawnCard = m_engine->drawFromStock();

    qDebug() << "Izvucena karta:" << drawnCard->toString();

    drawnCard->setFaceUp(true);

    m_engine->addCardToWaste(drawnCard);

    updateVisualsAfterDraw();

    emit stockUpdated(m_engine->getStockSize());
    emit wasteUpdated(m_engine->getWasteSize());
    emit gameStateChanged();

    qDebug() << "Karta izvucena. Stock:" << m_engine->getStockSize()
        << "Waste:" << m_engine->getWasteSize();
}

void GameManager::updateVisualsAfterDraw() {
    clearCardItems();
    createCardItems();
    connectCardSignals();

    qDebug() << "Vizueli azurirani nakon vucenja karte";
}

void GameManager::updateVisualsAfterStockReset() {
    qDebug() << "=== updateVisualsAfterStockReset ===";

    PileItem* stockPileItem = nullptr;
    PileItem* wastePileItem = nullptr;

    for (auto* pileItem : m_pileItems) {
        if (!pileItem) continue;

        if (pileItem->getPileType() == PileItem::Stock) {
            stockPileItem = pileItem;
        }
        else if (pileItem->getPileType() == PileItem::Waste) {
            wastePileItem = pileItem;
        }
    }

    if (!stockPileItem || !wastePileItem) {
        qWarning() << "Nema stock ili waste pile item!";
        return;
    }

    qDebug() << "Pronadjeni pile items: Stock =" << stockPileItem
        << "Waste =" << wastePileItem;

    qDebug() << "Cistim waste pile...";
    auto wasteCardItems = wastePileItem->getCardItems();
    for (CardItem* cardItem : wasteCardItems) {
        m_cardItems.removeAll(cardItem);

        for (auto it = m_cardMap.begin(); it != m_cardMap.end(); ) {
            if (it.value() == cardItem) {
                it = m_cardMap.erase(it);
            }
            else {
                ++it;
            }
        }
        if (cardItem->scene()) {
            cardItem->scene()->removeItem(cardItem);
        }
        delete cardItem;
    }

    qDebug() << "Cistim stock pile...";
    stockPileItem->clearCardItems();

    qDebug() << "Cistim GameManager mape...";

    QList<CardItem*> toRemove;

    for (CardItem* cardItem : m_cardItems) {
        if (!cardItem) continue;

        if (auto* parentPile = dynamic_cast<PileItem*>(cardItem->parentItem())) {
            if (parentPile == stockPileItem || parentPile == wastePileItem) {
                toRemove.append(cardItem);
            }
        }
    }

    for (CardItem* cardItem : toRemove) {
        m_cardItems.removeAll(cardItem);

        auto card = cardItem->getCard();
        if (card) {
            m_cardMap.remove(card);
        }
    }

    qDebug() << "Kreiram nove CardItem-e za stock...";

    auto stockPile = m_engine->stockPile();
    if (stockPile) {
        auto cards = stockPile->cards();
        qDebug() << "Stock ima" << cards.size() << "karata";

        int zValue = 0;
        for (const auto& card : cards) {
            if (!card) continue;

            card->setFaceUp(false);

            auto* cardItem = new CardItem(card, stockPileItem);
            cardItem->updateAppearance();

            cardItem->setPos(0, 0);
            cardItem->setZValue(zValue++);

            m_cardItems.append(cardItem);
            m_cardMap[card] = cardItem;

            qDebug() << "Kreiran CardItem za:" << card->toString();
        }
    }

    stockPileItem->updateLayout();
    wastePileItem->updateLayout();
    if (m_scene) {
        m_scene->update();
    }

    qDebug() << "Vizueli azurirani nakon stock reset";
    qDebug() << "Ukupno CardItem-a:" << m_cardItems.size();
    qDebug() << "CardMap velicina:" << m_cardMap.size();
}

void GameManager::undo() {
    if (m_engine->canUndo()) {
        m_engine->undo();

        clearCardItems();
        createCardItems();
        connectCardSignals();

        emit gameStateChanged();
        emit undoAvailable(m_engine->canUndo());
        emit redoAvailable(m_engine->canRedo());
    }
}

void GameManager::redo() {
    if (m_engine->canRedo()) {
        m_engine->redo();

        clearCardItems();
        createCardItems();
        connectCardSignals();

        emit gameStateChanged();
        emit undoAvailable(m_engine->canUndo());
        emit redoAvailable(m_engine->canRedo());
    }
}

bool GameManager::canUndo() const {
    return m_engine ? m_engine->canUndo() : false;
}

bool GameManager::canRedo() const {
    return m_engine ? m_engine->canRedo() : false;
}

CardItem* GameManager::getCardItemForCard(std::shared_ptr<Card> card) {
    return m_cardMap.value(card, nullptr);
}

PileItem* GameManager::getPileItemForPile(std::shared_ptr<Pile> pile) {
    return m_pileMap.value(pile, nullptr);
}

PileItem* GameManager::getPileItemForCard(CardItem* cardItem) {
    if (!cardItem) {
        qWarning() << "getPileItemForCard: cardItem je null!";
        return nullptr;
    }

    if (auto* parentPileItem = dynamic_cast<PileItem*>(cardItem->parentItem())) {
        return parentPileItem;
    }

    auto card = cardItem->getCard();
    if (!card) {
        qWarning() << "getPileItemForCard: cardItem nema Card!";
        return nullptr;
    }

    qDebug() << "Trazim pile za karticu:" << card->toString();

    for (PileItem* pileItem : m_pileItems) {
        if (!pileItem) continue;

        auto pile = pileItem->getPile();
        if (!pile) continue;

        auto cards = pile->cards();
        for (const auto& c : cards) {
            if (c == card) {
                qDebug() << "  Pronadjen u pile tipa:" << pileItem->getPileType();
                return pileItem;
            }
        }
    }

    qWarning() << "Karta" << card->toString() << "nije u bilo kojem pile-u!";
    return nullptr;
}