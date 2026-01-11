#ifndef PILEITEM_H
#define PILEITEM_H

#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <memory>
#include "../header/pile.h"
#include "carditem.h"

class PileItem : public QGraphicsRectItem {
public:
    enum PileType { Stock, Waste, Foundation, Table };

    PileItem(std::shared_ptr<Pile> pile, PileType type, QGraphicsItem* parent = nullptr);

    std::shared_ptr<Pile> getPile() const { return m_pile; }
    PileType getPileType() const { return m_type; }

    void addCardItem(CardItem* cardItem);
    void removeCardItem(CardItem* cardItem);
    void clearCardItems();
    QList<CardItem*> getCardItems() const { return m_cardItems; }
    void updateLayout();
    void clearCards();

    bool isEmpty() const { return m_cardItems.isEmpty(); }
    CardItem* topCardItem() const {
        return m_cardItems.isEmpty() ? nullptr : m_cardItems.last();
    }
    bool isTopCard(CardItem* cardItem) const;
    QPointF getNextCardPosition() const;

    void setHighlight(bool highlight);
    bool isHighlighted() const { return m_isHighlighted; }
    void setHighlightColor(const QColor& color);

private:
    bool m_isHighlighted;
    QColor m_highlightColor;
    QColor m_originalBrushColor;
    QColor m_originalPenColor;

    void setupAppearance();

    std::shared_ptr<Pile> m_pile;
    PileType m_type;
    QList<CardItem*> m_cardItems;
    static constexpr int CARD_OFFSET = 20;
};

#endif