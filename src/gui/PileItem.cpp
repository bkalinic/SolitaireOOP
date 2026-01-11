#include "../guiHeader/PileItem.h"
#include <QPainter>
#include <QGraphicsScene>

PileItem::PileItem(std::shared_ptr<Pile> pile, PileType type, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
    , m_pile(pile)
    , m_type(type) 
    , m_isHighlighted(false)
    , m_highlightColor(QColor(255, 255, 0, 100))
    , m_originalBrushColor(Qt::lightGray)
    , m_originalPenColor(Qt::black) {

    setupAppearance();
    setRect(0, 0, CardItem::getCardSize().width(), CardItem::getCardSize().height());

    m_originalBrushColor = brush().color();
    m_originalPenColor = pen().color();
}

void PileItem::setHighlight(bool highlight) {
    if (m_isHighlighted == highlight) {
        return;
    }
    m_isHighlighted = highlight;

    if (highlight) {
        QBrush highlightBrush(m_highlightColor);
        setBrush(highlightBrush);

        QPen highlightPen(Qt::yellow, 3);
        setPen(highlightPen);

        setZValue(10);
    }
    else {
        setBrush(QBrush(m_originalBrushColor));
        setPen(QPen(m_originalPenColor, 2));

        setZValue(-1);

        setGraphicsEffect(nullptr);
    }

    update();
}

void PileItem::setHighlightColor(const QColor& color) {
    m_highlightColor = color;

    if (m_isHighlighted) {
        setBrush(QBrush(m_highlightColor));
    }
}

void PileItem::setupAppearance() {
    QPen borderPen(Qt::black, 2);
    QBrush backgroundBrush(Qt::lightGray);

    switch (m_type) {
    case Stock:
        backgroundBrush = QBrush(QColor(220, 220, 220));
        break;
    case Waste:
        backgroundBrush = QBrush(QColor(240, 240, 240));
        break;
    case Foundation:
        borderPen.setStyle(Qt::DashLine);
        backgroundBrush = QBrush(QColor(255, 255, 200));
        break;
    case Table:
        backgroundBrush = Qt::NoBrush;
        break;
    }

    setPen(borderPen);
    setBrush(backgroundBrush);
    setZValue(-1);
}

void PileItem::addCardItem(CardItem* cardItem) {
    if (cardItem && !m_cardItems.contains(cardItem)) {
        m_cardItems.append(cardItem);
        cardItem->setParentItem(this);
        updateLayout();
    }
}

void PileItem::removeCardItem(CardItem* cardItem) {
    if (cardItem) {
        m_cardItems.removeAll(cardItem);
        cardItem->setParentItem(nullptr);
        updateLayout();
    }
}

void PileItem::clearCardItems() {
    qDebug() << "PileItem::clearCardItems() - brisem" << m_cardItems.size() << "karata";

    for (CardItem* cardItem : m_cardItems) {
        if (cardItem) {
            if (cardItem->scene()) {
                cardItem->scene()->removeItem(cardItem);
            }
            delete cardItem;
        }
    }

    m_cardItems.clear();
    qDebug() << "Sve karte obrisane iz pile-a";
}

void PileItem::clearCards() {
    for (CardItem* card : m_cardItems) {
        card->setParentItem(nullptr);
    }
    m_cardItems.clear();
}

bool PileItem::isTopCard(CardItem* cardItem) const {
    return !m_cardItems.empty() && m_cardItems.last() == cardItem;
}

void PileItem::updateLayout() {
    QPointF position(0, 0);

    if (m_type == Table && !m_cardItems.isEmpty()) {
        for (int i = 0; i < m_cardItems.size(); i++) {
            int yOffset = i * 25;
            m_cardItems[i]->setPos(0, yOffset);
            m_cardItems[i]->setZValue(i);
        }
    }
    else {
        for (int i = 0; i < m_cardItems.size(); i++) {
            m_cardItems[i]->setPos(0, 0);
            m_cardItems[i]->setZValue(i);
        }
    }
}

QPointF PileItem::getNextCardPosition() const {
    if (m_type == Table && !m_cardItems.isEmpty()) {
        return QPointF(0, m_cardItems.size() * CARD_OFFSET);
    }
    return QPointF(0, 0);
}